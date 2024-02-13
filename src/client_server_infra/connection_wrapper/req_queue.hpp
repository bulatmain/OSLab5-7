#ifndef REQ_QUEUE_HPP
#define REQ_QUEUE_HPP

#include "msg_queue.hpp"
#include "auxiliary_entities/push_connection.hpp"

namespace lab5_7 {
    class ReqQueue : public MsgQueue {
    protected:
        using thread_ptr = std::shared_ptr<std::thread>;
        using socket_ptr = std::shared_ptr<PushConnection>;
        using flag_ptr = std::shared_ptr<FlagVO>;

        thread_ptr dispatch_thread_ptr;
        socket_ptr pusher;
        flag_ptr keep_running = std::make_shared<FlagVO>(false);


    public:
        void set_endpoint(std::string const& endpoint);
        void set_endpoint(std::string&& endpoint);

        void connect();
        void disconnect();
         
        void launch();
        void stop();

    };
};

#endif