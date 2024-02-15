#ifndef PULL_QUEUE_HPP
#define PULL_QUEUE_HPP

#include "msg_queue.hpp"

namespace lab5_7 {
    class PullQueue : public MsgQueue {
    protected:
        PullQueue() : MsgQueue() {}
    
    public:

        void pull(zmq::message_t& msg) {
            que_ptr->wait_and_pop(msg);
        }

        bool try_pull(zmq::message_t& msg) {
            return que_ptr->try_pop(msg);
        }

    };
}

#endif