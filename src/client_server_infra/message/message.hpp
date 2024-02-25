#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "message_types.hpp"
#include "aux_ser_funcs.hpp"

#include <memory>
#include <string>
#include <iostream>

namespace lab5_7 {
    class Message {
    public:
        using msg_ptr = std::shared_ptr<Message>;

        template <typename T, typename... Args>    
        static msg_ptr construct(Args... args) {
            static_assert(
                std::is_base_of<Message, T>::value &&
                !std::is_abstract<T>::value,
                "T must be a derived class of Message in Context<T>.");
            return std::make_shared<T>(args...);
        }
        
        // String format: <Derived_1>{<Derived_1>{<Derived_2>{...Derived_n{...}...}}}
        virtual std::string serialize() const final {
            std::string msg_str;
            add_message_header(msg_str);
            serialize_message(msg_str);
            complete_serialization(msg_str);
            return msg_str;
        }

        static msg_ptr deserialize(std::string& msg_str);
        static msg_ptr deserialize(std::string&& msg_str);
        static msg_ptr deserialize(std::string const& msg_str);
        static msg_ptr deserializeUnpacked(std::string& msg_str);

        virtual ~Message() = 0;

    protected:
        void add_message_header(std::string& msg_str) const {
            msg_str += "Message{";
        }

        virtual void serialize_message(std::string& msg_str) const = 0;
        
    };

    Message::~Message() {}

    template <typename T, typename... Args>
    std::shared_ptr<T> constructMessageType(Args... args) {
        return std::dynamic_pointer_cast<T>(Message::construct<T>(args...));
    }

};

#endif