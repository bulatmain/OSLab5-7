#ifndef SERVER_VALIDATOR_HPP
#define SERVER_VALIDATOR_HPP

#include <infrastructure/connection_wrapper/connection_module.hpp>
#include <aux_network_funcs/get_ip_func/get_ip_func.hpp>

namespace lab5_7 {
    class Validator final {
    public:
        using vdtr_ptr = std::shared_ptr<Validator>;

    protected:
        using flag_ptr = std::shared_ptr<FlagVO const>;
        using eventsToHandleList = std::shared_ptr<ConcurrentQueue<Message>>;

        PullQueue puller;
        flag_ptr keep_running;
        eventsToHandleList eventsList;
        

    public:
        Validator(std::string const& endpoint, eventsToHandleList eventsList, flag_ptr keep_running)
            : puller(endpoint), eventsList(eventsList), keep_running(keep_running) {}

        template <class... Args>
        static vdtr_ptr makePointer(Args... args) {
            return std::make_shared<Validator>(args...);
        } 

        void launch() {
            puller.bind();
            puller.runRecieveThread();
            while (keep_running->getValue()) {
                auto msg = puller.pull();
                if (msgIsRequest(msg)) {
                    eventsList->push(msg);
                }
            }
        }

        bool msgIsRequest(Message::msg_ptr msg) {
            auto p = std::dynamic_pointer_cast<Request>(msg);
            return static_cast<bool>(p);
        }

    };
};

#endif