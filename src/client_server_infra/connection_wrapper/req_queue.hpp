#ifndef REQ_QUEUE_HPP
#define REQ_QUEUE_HPP

#include "msg_queue.hpp"
#include "value_objects.hpp"

namespace lab5_7 {
    class ReqQueue : public MsgQueue {
    protected:
        using thread_ptr = std::shared_ptr<std::thread>;
        using flag_ptr = std::shared_ptr<FlagVO>;
        using address_ptr = std::shared_ptr<EndpointVO>;

        thread_ptr dispatch_thread_ptr;
        flag_ptr connected = std::make_shared<FlagVO>(false);
        flag_ptr keep_running = std::make_shared<FlagVO>(false);
        address_ptr endpoint_ptr;

    public:
        void set_endpoint(std::string const& endpoint) {
            this->endpoint_ptr = std::make_shared<EndpointVO>(endpoint);
        }

        void set_endpoint(std::string&& endpoint) {
            this->endpoint_ptr = std::make_shared<EndpointVO>(std::move(endpoint));
        }

        void connect();
        void disconnect();
         
        void launch();
        void stop();

    };
};

#endif