#ifndef MSG_QUEUE_HPP
#define MSG_QUEUE_HPP

#include "concurrent_queue.hpp"
#include "zmq.hpp"

namespace lab5_7 {
    class MsgQueue {
    protected:
        using queue = ConcurrentQueue<zmq::message_t>;
        using queue_ptr = std::shared_ptr<queue>;

        queue_ptr que_ptr;

    public:
        MsgQueue() : que_ptr(std::make_shared<queue>()) {}

        void push(zmq::message_t&& msg) {
            que_ptr->push(msg);
        }

        void clear() {
            que_ptr->clear();
        }

        std::queue<zmq::message_t> copy_queue() const {
            return que_ptr->copy_queue();
        }

        std::queue<zmq::message_t> unload() {
            return que_ptr->unload();
        }

    };

};

#endif