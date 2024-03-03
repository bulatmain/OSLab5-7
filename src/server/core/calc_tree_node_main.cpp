#include <iostream>

#include "server_entity/calc_tree_node.hpp"
#include <fstream>

typedef std::chrono::system_clock timer;

using namespace std::chrono_literals;

using namespace lab5_7;

class CalcTreeNode {
public:
    using thread_ptr = std::shared_ptr<std::thread>;
    using flag_ptr = std::shared_ptr<FlagVO>;
    using calcNodeRoot_ptr = std::shared_ptr<CalcNodeRoot>;

    std::string const serverEndpoint;
    std::string const endpoint;
    size_t id;
    flag_ptr keep_running;
    timer::duration checkingHeartbeatPeriod;
    
    PullQueue eventsList;
    PushQueue ancestorPusher;
    std::list<calcNodeRoot_ptr> children;

    std::list<CommandRecord> commandRecordsList;

    CalcTreeNode(std::string const& serverEndpoint, std::string const& endpoint, size_t id, timer::duration checkingHeartbeatPeriod)
        :   serverEndpoint(serverEndpoint), 
            endpoint(endpoint), id(id), 
            keep_running(std::make_shared<FlagVO>(false)), 
            checkingHeartbeatPeriod(checkingHeartbeatPeriod),
            eventsList(endpoint),
            ancestorPusher(serverEndpoint) {
        std::cout << "(from new process):\tAncestor endpoint: " << serverEndpoint << "\n\tNode endpoint: " << endpoint << "\n";
        launchMessageQueues();
    }

    void launch() {
        std::cout << "!! CalcTreeNode launched, pid: " << getpid() << "\n";
        keep_running->setValue(true);
        timer::time_point checkingHeatbeatPeriodStart = timer::now();
        while (keep_running->getValue()) {
            pushMessage<Heartbeat>(id, timer::now());
            bool active = false;
            if (!eventsList.empty()) {
                auto msg = eventsList.pull();
                handle(msg);
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

    void stop() {
        keep_running->setValue(false);
    }

    ~CalcTreeNode() {
        stop();
        directKillSignal();
    }

protected:
    void launchMessageQueues() {
        eventsList.bind();
        eventsList.runRecieveThread();
        ancestorPusher.connect();
        ancestorPusher.runDispatchThread();
    }

    void directKillSignal() {
        auto resp = Message::construct<KillSignal>();
        redirectToNodes(resp);
    }

    void handle(Message::msg_ptr msg) {
        if (isTypeMsg<Command>(msg)) {
            handleCommand(std::dynamic_pointer_cast<Command>(msg));
        } else if (isTypeMsg<KillSignal>(msg)) {
            handleKillSignal(std::dynamic_pointer_cast<KillSignal>(msg));
        } else if (isTypeMsg<Response>(msg)) {
            handleResponse(std::dynamic_pointer_cast<Response>(msg));
        }
    }

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

    void handleExec(std::shared_ptr<CommandExec> cmd) {
        auto const& id = cmd->nodeId;
        if (id == this->id) {
            executeHere(cmd);
        } else {
            redirectToNodes(cmd);
            commandRecordsList.emplace_back(CommandRecord(ExecEnum));
        }
    }

    void handleKillSignal(std::shared_ptr<KillSignal> cmd) {
        redirectToNodes(cmd);
        stop();
    }

    void handleHeartbeat(std::shared_ptr<Heartbeat> cmd) {
        auto id = cmd->nodeId;
        auto lastHeartbeatTime = cmd->lastHeartbeatTime;
        for (auto& node : children) {
            if (node->id == id) {
                node->lastHeartBeat = lastHeartbeatTime;
                return;
            }
        }
    }

    void handleResponse(Response::resp_ptr resp) {
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

    void executeHere(std::shared_ptr<CommandExec> cmd) {
        double sum = 0;
        for (auto const& e : cmd->k) {
            sum += e;
        }
        pushMessage<GoodExecResp>(id, sum);
    }

    void createRootNodeAndPushResponse(std::shared_ptr<CommandCreate> cmd) {
        auto nodeEndpoint = SelfIPInfo::generateSelfEndpoint();
        auto newNodeId = cmd->newNodeId;
        tryPushBackNewNode(nodeEndpoint, newNodeId);
    }

    void tryPushBackNewNode(std::string const& nodeEndpoint, uint64_t newNodeId) {
        try {
            children.emplace_back(std::make_shared<CalcNodeRoot>(endpoint, nodeEndpoint, newNodeId, checkingHeartbeatPeriod));
        } catch (...) {
            pushMessage<BadCreateResp>(id, newNodeId);
            return;
        }
        auto pid = getProcessPidByNodeId(newNodeId);
        pushMessage<GoodCreateResp>(newNodeId, pid);
    }

    bool timeToCheckHeartbeat(timer::time_point& checkingHeatbeatPeriodStart) {
        auto timeNow = timer::now();
        auto timePassed =  timeNow - checkingHeatbeatPeriodStart;
        auto res = checkingHeartbeatPeriod < timePassed;
        if (res) {
            checkingHeatbeatPeriodStart = timeNow;
        }
        return res;
    }

    void checkHeartbeat() {
        for (auto node = children.begin(); node != children.end(); ++node) {
            auto timeNow = timer::now();
            auto timePassed = timeNow - (*node)->lastHeartBeat;
            if (checkingHeartbeatPeriod < timePassed) {
                pushMessage<NodeIsDeadResponse>((*node)->id);
                node = children.erase(node);
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

    uint64_t getProcessPidByNodeId(uint64_t id) {
        for (auto const& node : children) {
            if (node->id == id) {
                return node->pid;
            }
        }
    }

    template <typename T, typename... Args>
    void pushMessage(Args... args) {
        auto msg = Message::construct<T>(args...);
        redirectToAncestor(msg);
    }

    void redirectToNodes(Message::msg_ptr msg) {
        if (children.empty()) {
            handleDeadEnd(msg);
        } else {
            for (auto& node : children) {
                node->push(msg);
            }
        }
    }

    void redirectToAncestor(Message::msg_ptr msg) {
        ancestorPusher.push(msg);
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
        return (i.recievedResponses == children.size());
    }
};



int main(int argc, char** argv) {
    if (argc != 5) {
        std::cout << "Error: CalcTreeNodeMain invalid arguments!\n";
        return -1;
    }

    std::string serverEndpoint(argv[1]);
    std::string endpoint(argv[2]);
    size_t id = std::stoull(argv[3]);
    timer::duration checkingHeartbeatPeriod = std::chrono::nanoseconds(std::stoull(argv[4]));

    CalcTreeNode node(serverEndpoint, endpoint, id, checkingHeartbeatPeriod);

    node.launch();

    return 0;
}