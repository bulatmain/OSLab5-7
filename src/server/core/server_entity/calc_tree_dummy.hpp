#include "calc_tree_node.hpp"

namespace lab5_7 {

class CalcTreeInterface {
protected:
    typedef std::chrono::system_clock timer;
    using flag_ptr = std::shared_ptr<FlagVO const>;
    using eventsToHandleList = std::shared_ptr<ConcurrentQueue<Message>>;
    using commandRequestQueue = std::shared_ptr<ConcurrentQueue<Command>>;

    eventsToHandleList eventsList;
    flag_ptr keep_running;
    commandRequestQueue commandsQueue;
    timer::duration checkingHeartbeatPeriod;

public:
    CalcTreeInterface(eventsToHandleList eventsList, flag_ptr keep_running, timer::duration time)
        :   eventsList(eventsList), 
            keep_running(keep_running),
            commandsQueue(std::make_shared<ConcurrentQueue<Command>>()),
            checkingHeartbeatPeriod(4*time) {}

    void push(Command::cmd_ptr cmd) {
        commandsQueue->push(cmd);
    }

    friend class CalcTree;

};


class CalcTree {
public:
    using ct_ptr = std::shared_ptr<CalcTree>;

protected:
    typedef std::chrono::system_clock timer;
    using flag_ptr = std::shared_ptr<FlagVO const>;
    using eventsToHandleList = std::shared_ptr<ConcurrentQueue<Message>>;
    using commandRequestQueue = std::shared_ptr<ConcurrentQueue<Command>>;
    using calcNodeRoot_ptr = std::shared_ptr<CalcNodeRoot>;

    size_t const id = 0;

    std::string const endpoint;
    eventsToHandleList eventsList;
    flag_ptr keep_running;
    commandRequestQueue commandsQueue;
    std::list<calcNodeRoot_ptr> rootNodesList;
    PullQueue puller;
    timer::duration checkingHeartbeatPeriod;

    std::list<CommandRecord> commandRecordsList;


public:
    static ct_ptr makePointer(CalcTreeInterface& ctInterface) {
        return std::make_shared<CalcTree>(
            std::move(ctInterface.eventsList), 
            std::move(ctInterface.keep_running),
            ctInterface.commandsQueue,
            ctInterface.checkingHeartbeatPeriod
        );
    }

    CalcTree(   eventsToHandleList&& eventsList, 
                flag_ptr&& keep_running, 
                commandRequestQueue commandsQueue,
                timer::duration checkingHeartbeatPeriod)
        :   endpoint(SelfIPInfo::generateSelfEndpoint()),
            eventsList(std::move(eventsList)), 
            keep_running(std::move(keep_running)),
            commandsQueue(commandsQueue),
            puller(endpoint),
            checkingHeartbeatPeriod(checkingHeartbeatPeriod) {
        puller.bind();
        puller.runRecieveThread();
    }


    void launch() {
        std::cout << "<> Dummy launched!\n";
        timer::time_point checkingHeatbeatPeriodStart = timer::now();
        while (keep_running->getValue()) {
            bool active = false;
            if (!commandsQueue->empty()) {
                auto cmd = commandsQueue->wait_and_pop();
                handleCommand(cmd);
                active = true;
            }
            if (!puller.empty()) {
                auto resp = puller.pull();
                handleResponse(resp);
                active = true;
            }
            if (timeToCheckHeartbeat(checkingHeatbeatPeriodStart)) {
                checkHeartbeat();
                active = true;
            }
            if (!active) {
                std::this_thread::sleep_for(100ms);
            }
        }
    }

    ~CalcTree() {
        auto killSignal = Message::construct<KillSignal>();
        redirectToNodes(killSignal);
    }

protected:
    void handleCommand(Command::cmd_ptr cmd) {
        if (isTypeMsg<CommandCreate>(cmd)) {
            handleCreate(std::dynamic_pointer_cast<CommandCreate>(cmd));
        } else if (isTypeMsg<CommandExec>(cmd)) {
            handleExec(std::dynamic_pointer_cast<CommandExec>(cmd));
        }
    }

    void handleCreate(std::shared_ptr<CommandCreate> cmd) {
        auto newNodeId = cmd->newNodeId;
        if (newNodeId == id) {
            pushMessage<NodeExistResponse>(id);
            return;
        }
        auto parentId = cmd->parentId;
        if (parentId == id) {
            createRootNodeAndPushResponse(cmd);
        } else {
            redirectToNodes(cmd);
            commandRecordsList.emplace_back(CommandRecord(CreateEnum));
        }
    }   

    void handleHeartbeat(std::shared_ptr<Heartbeat> cmd) {
        auto id = cmd->nodeId;
        auto lastHeartbeatTime = cmd->lastHeartbeatTime;
        for (auto& node : rootNodesList) {
            if (node->id == id) {
                node->lastHeartBeat = lastHeartbeatTime;
                return;
            }
        }
    }

    void handleResponse(Message::msg_ptr resp) {
        if (isTypeMsg<BadExecResp>(resp)) {
            accountBadExecResp(std::dynamic_pointer_cast<BadExecResp>(resp));
        } else if (isTypeMsg<GoodExecResp>(resp)) {
            accountGoodExecResp(std::dynamic_pointer_cast<GoodExecResp>(resp));
        } else if (isTypeMsg<BadCreateResp>(resp)) {
            accountBadCreateResp(std::dynamic_pointer_cast<BadCreateResp>(resp));
        } else if (isTypeMsg<GoodCreateResp>(resp)) {
            accountGoodCreateResp(std::dynamic_pointer_cast<GoodCreateResp>(resp));
        } else if (isTypeMsg<NodeExistResponse>(resp)) {
            closeRecordAndRedirectResp(std::dynamic_pointer_cast<NodeExistResponse>(resp));
        } else if (isTypeMsg<Heartbeat>(resp)) {
            handleHeartbeat(std::dynamic_pointer_cast<Heartbeat>(resp));
        } else {
            redirectToAncestor(resp);
        }
    }

    void createRootNodeAndPushResponse(std::shared_ptr<CommandCreate> cmd) {
        auto nodeEndpoint = SelfIPInfo::generateSelfEndpoint();
        auto newNodeId = cmd->newNodeId;
        tryPushBackNewNode(nodeEndpoint, newNodeId);
    }


    bool timeToCheckHeartbeat(timer::time_point& checkingHeatbeatPeriodStart) {
        auto timeNow = timer::now();
        auto timePassed = timeNow - checkingHeatbeatPeriodStart;
        auto res = checkingHeartbeatPeriod < timePassed;
        if (res) {
            checkingHeatbeatPeriodStart = timeNow;
        }
        return res;
    }

    void checkHeartbeat() {
        std::list<std::list<CalcNodeRoot>::iterator> deadNodes;
        for (auto node = rootNodesList.begin(); node != rootNodesList.end(); ++node) {
            auto timeNow = timer::now();
            auto timePassed = timeNow - (*node)->lastHeartBeat;
            if (checkingHeartbeatPeriod < timePassed) {
                pushMessage<NodeIsDeadResponse>((*node)->id);
                node = rootNodesList.erase(node);
            }
        }
    }


    void accountBadExecResp(std::shared_ptr<BadExecResp> resp) {
        auto i = findRecord(ExecEnum);
        i->accountBadResp();
        if (recordIsBad(*i)) {
            commandRecordsList.erase(i);
            redirectToAncestor(resp);
        }
    }

    void accountGoodExecResp(std::shared_ptr<GoodExecResp> resp) {
        auto i = findRecord(ExecEnum);
        i->accountGoodResp();
        if (recordIsFull(*i)) {
            commandRecordsList.erase(i);
        }
        redirectToAncestor(resp);      
    }

    void accountBadCreateResp(std::shared_ptr<BadCreateResp> resp) {
        auto i = findRecord(CreateEnum);
        i->accountBadResp();
        if (recordIsBad(*i)) {
            commandRecordsList.erase(i);
            redirectToAncestor(resp);          
        }
    }

    void accountGoodCreateResp(std::shared_ptr<GoodCreateResp> resp) {
        auto i = findRecord(CreateEnum);
        i->accountGoodResp();
        if (recordIsFull(*i)) {
            commandRecordsList.erase(i);
        }
        redirectToAncestor(resp);          
    }

    void closeRecordAndRedirectResp(std::shared_ptr<NodeExistResponse> resp) {
        auto i = findRecord(CreateEnum);
        commandRecordsList.erase(i);
        redirectToAncestor(resp);  
    }

    std::list<CommandRecord>::iterator findRecord(CommandType type) {
        auto i = commandRecordsList.begin();
        while (i->type != type) { ++i; }
        return i;
    }

    bool recordIsBad(CommandRecord const& i) {
        return recordIsFull(i) && !(i.wasGoodResponse);
    }

    bool recordIsFull(CommandRecord const& i) {
        return (i.recievedResponses == rootNodesList.size());
    }

    void tryPushBackNewNode(std::string const& nodeEndpoint, uint64_t newNodeId) {
        try {
            rootNodesList.emplace_back(std::make_shared<CalcNodeRoot>(endpoint, nodeEndpoint, newNodeId, checkingHeartbeatPeriod));
        } catch (...) {
            pushMessage<BadCreateResp>(id, newNodeId);
            return;
        }
        auto pid = getProcessPidByNodeId(newNodeId);
        pushMessage<GoodCreateResp>(newNodeId, pid);
    }

    uint64_t getProcessPidByNodeId(uint64_t id) {
        for (auto const& node : rootNodesList) {
            if (node->id == id) {
                return node->pid;
            }
        }
        throw std::runtime_error("Error: no nodes with such id in rootNodesList");
    }

    void handleExec(std::shared_ptr<CommandExec> cmd) {
        commandRecordsList.emplace_back(CommandRecord(ExecEnum));
        redirectToNodes(cmd);
    }

    void redirectToNodes(Message::msg_ptr msg) {
        if (rootNodesList.empty()) {
            handleDeadEnd(msg);
        } else {
            for (auto& node : rootNodesList) {
                node->push(msg); 
            }
        }
    }

    void handleDeadEnd(Message::msg_ptr msg) {
        if (isTypeMsg<CommandExec>(msg)) {
            handleCommandExecDeadEnd(std::dynamic_pointer_cast<CommandExec>(msg));
        } else if (isTypeMsg<CommandCreate>(msg)) {
            handleCommandCreateDeadEnd(std::dynamic_pointer_cast<CommandCreate>(msg));
        }
    }

    void handleCommandExecDeadEnd(std::shared_ptr<CommandExec> cmd) {
        auto id = cmd->nodeId;
        pushMessage<BadExecResp>(id);
    }

    void handleCommandCreateDeadEnd(std::shared_ptr<CommandCreate> cmd) {
        auto parentId  = cmd->parentId;
        auto newNodeId = cmd->newNodeId;
        pushMessage<BadCreateResp>(parentId, newNodeId);
    }

    template <typename T, typename... Args>
    void pushMessage(Args... args) {
        auto msg = Message::construct<T>(args...);
        std::cout << "Message pushed to ancestor: " << serialize(msg) << "\n";
        redirectToAncestor(msg);
    }

    void redirectToAncestor(Message::msg_ptr msg) {
        eventsList->push(msg);
    }

};

};