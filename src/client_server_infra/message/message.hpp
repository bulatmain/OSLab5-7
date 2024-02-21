#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "message_types.hpp"

#include <memory>
#include <string>

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
        
        // String format: Message{<Derived_1>{<Derived_2>{...Derived_n{...}...}}}
        virtual std::string serialize() const final {
            std::string msg_str;
            add_message_header(msg_str);
            serialize_message(msg_str);
            complete_serialization(msg_str);
            return msg_str;
        }

        static msg_ptr deserialize(std::string& req_str);
        static msg_ptr deserialize(std::string&& req_str);
        static msg_ptr deserialize(std::string const& req_str);

        virtual ~Message() = 0;

    protected:
        template <typename First, typename... Rest>
        void serializeWithArguments(std::string& req_str, First const& arg, Rest const&... args) const;

        virtual void add_message_header(std::string& msg_str) const final {
            msg_str += "Message{";
        }

        virtual void serialize_message(std::string& msg_str) const = 0;

        virtual void add_message_type(std::string& msg_str) const = 0;

    };

    Message::~Message() {}
};

#endif