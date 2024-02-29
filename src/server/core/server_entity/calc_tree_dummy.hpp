#include <infrastructure/connection_wrapper/connection_module.hpp>

namespace lab5_7 {



class CalcTreeInterface {
public:
    using flag_ptr = std::shared_ptr<FlagVO const>;
    using eventsToHandleList = std::shared_ptr<ConcurrentQueue<Message>>;
    using commandRequestQueue = std::shared_ptr<ConcurrentQueue<Command>>;

    eventsToHandleList eventsList;
    flag_ptr keep_running;
    commandRequestQueue commandQueue;

    CalcTreeInterface(eventsToHandleList eventsList, flag_ptr keep_running)
        :   eventsList(eventsList), 
            keep_running(keep_running),
            commandQueue(std::make_shared<ConcurrentQueue<Command>>()) {}

    void push(Command::cmd_ptr cmd) {
        commandQueue->push(cmd);
    }

};

class CalcTree {
public:
    using ct_ptr = std::shared_ptr<CalcTree>;

    using flag_ptr = std::shared_ptr<FlagVO const>;
    using eventsToHandleList = std::shared_ptr<ConcurrentQueue<Message>>;
    using commandRequestQueue = std::shared_ptr<ConcurrentQueue<Command>>;

    eventsToHandleList eventsList;
    flag_ptr keep_running;
    commandRequestQueue commandQueue;

    static ct_ptr makePointer(CalcTreeInterface& ctInterface) {
        return std::make_shared<CalcTree>(
            ctInterface.eventsList, 
            ctInterface.keep_running,
            ctInterface.commandQueue
        );
    }

    CalcTree(eventsToHandleList eventsList, flag_ptr keep_running, commandRequestQueue commandQueue)
        :   eventsList(eventsList), 
            keep_running(keep_running),
            commandQueue(commandQueue) {}


    void launch() {
        std::cout << "Dummy launched!\n";
        while (keep_running->getValue()) {
            auto cmd = commandQueue->wait_and_pop();
            auto resp = Message::construct<InvalidRequest>();
            eventsList->push(resp);
        }
    }

};

};