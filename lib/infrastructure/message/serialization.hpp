#ifndef MSG_SERIALIZATION_HPP
#define MSG_SERIALIZATION_HPP

#include "message.hpp"
#include "request/request.hpp"

namespace lab5_7 {
    MessageType defineMessageType(std::string& msg_str) {
        std::string msg_type = extractType(msg_str);
        if (msg_type == "Request") {
            return Request;
        } /*else if (msg_type == "Responce") {
            return Responce;
        }*/ else {            
            throw std::invalid_argument("Error, initiated at defineMessageType(...): can not define Message type");
        }
    }

    Message::msg_ptr deserializeType(MessageType type, std::string& msg_type) {
        if (type == Request) {
            return Request::deserializeUnpacked(msg_type);
        } /*else if (type == Response) {
            return Response::deserialize(std::move(msg_str));
        }*/ else {
            throw std::invalid_argument("Wtf?");
        }
    }

    Message::msg_ptr Message::deserializeUnpacked(std::string& msg_str) {
        MessageType type = defineMessageType(msg_str);
        return deserializeType(type, msg_str);
    }

    Message::msg_ptr Message::deserialize(std::string& msg_str) {
        if (extractType(msg_str) != "Message") {
            throw std::invalid_argument("Error: attempt to deserialize invalid message string");
        }
        return deserializeUnpacked(msg_str);
    }

    Message::msg_ptr Message::deserialize(std::string&& msg_str) {
        auto _msg_str = std::move(msg_str);
        return std::move(Message::deserialize(_msg_str));
    }

    Message::msg_ptr Message::deserialize(std::string const& msg_str) {
        auto _msg_str = msg_str;
        return std::move(Message::deserialize(std::move(_msg_str)));
    }

    std::string serialize(Message::msg_ptr msg) {
        return msg->serialize();
    }

};


#endif