#ifndef MSG_QUEUE_HPP
#define MSG_QUEUE_HPP

#include "concurrent_queue.hpp"
#include "msg_queue_exceptions.hpp"
#include <infrastructure/message/message_module.hpp>


namespace lab5_7 {
    class MsgQueue {
    protected:
        using queue = ConcurrentQueue<Message>;
        using queue_ptr = std::shared_ptr<queue>;

        queue_ptr que_ptr;
        
        MsgQueue() : que_ptr(std::make_shared<queue>()) {}

    public:
        void clear() {
            que_ptr->clear();
        }

        std::list<std::string> copy_msgs_content() const {
            return que_ptr->map<std::string>(to_string);
        }

        std::list<std::string> copy_msgs_content() {
            return const_cast<MsgQueue const*>(this)->copy_msgs_content();
        }

        std::list<std::string> unload() {
            auto list = std::move(copy_msgs_content());
            que_ptr->clear();
            return list;
        }


    protected:
        static std::string to_string(Message::msg_const_ptr msg) {
            return serialize(msg);
        }

    };

};

#endif