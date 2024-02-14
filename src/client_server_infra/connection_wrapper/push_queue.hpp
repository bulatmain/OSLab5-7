#ifndef PUSH_QUEUE_HPP
#define PUSH_QUEUE_HPP

#include "msg_queue.hpp"

namespace lab5_7 {
    class PushQueue : public MsgQueue {
    public:
        PushQueue() : MsgQueue() {}

        void push(zmq::message_t&& msg) {
            que_ptr->push(std::move(msg));
        }

        template <ConvertableToMsg T>
        void push(T&& obj) {
            que_ptr->push(to_msg(obj));
        }

    };
}

#endif