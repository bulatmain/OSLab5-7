#ifndef MSG_SERIALIZATION_HPP
#define MSG_SERIALIZATION_HPP

#include "message_module.hpp"

namespace lab5_7 {
    std::string extractType(std::string& ser_cmd) {
        std::size_t pos = ser_cmd.find('{');
        if (pos == std::string::npos) {
            return "";
        }
        auto command_type = ser_cmd.substr(0, pos); 
        ser_cmd = ser_cmd.substr(pos + 1, ser_cmd.size() - 1);
        return command_type;
    }

    MessageType defineMessageType(std::string& msg_str) {
        std::string msg_type = extractType(msg_str);
        if (msg_type == "Message") {
            return Request;
        } /*else if (msg_type == "Responce") {
            return Responce;
        }*/ else {            
            throw std::invalid_argument("Error, initiated at defineMessageType(...): can not define Message type");
        }
    }

    Message::msg_ptr deserializeType(MessageType type, std::string& msg_type) {
        if (type == Request) {
            return Request::deserialize(std::move(msg_type));
        } /*else if (type == Response) {
            return Response::deserialize(std::move(msg_str));
        }*/ else {
            throw std::invalid_argument("Wtf?");
        }
    }

    Message::msg_ptr Message::deserialize(std::string& msg_str) {
        MessageType type = defineMessageType(msg_str);
        return deserializeType(type, msg_str);
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

    template <typename T>
    void add_first_class_variable(std::string& msg_str, T const& var) {
        msg_str += "{" + std::to_string(var);
    }
    template <typename T>
    void add_next_class_variable(std::string& msg_str, T const& var) {
        msg_str += ", " + std::to_string(var);
    }
    void complete_serialization(std::string& msg_str) {
        msg_str += "}";
    }

    std::size_t find_start_of_class_vars(std::string const& msg_str) {
        return msg_str.find('{') + 1;
    }

    template <typename T>
    T getNextVar(std::string const& msg_str, std::size_t& l) {
        std::string var_str = getNextVarString(msg_str, l);
        return convert_string<T>(var_str);
    }

    std::string getNextVarString(std::string const& msg_str, std::size_t& l) {
        std::size_t r = findRightEdgeOfNextVar(msg_str, l);
        if (r == std::string::npos) {
            std::size_t l_ = msg_str.size(); std::swap(l, l_);
            return msg_str.substr(l_, msg_str.size() - l_);
        } else {
            std::size_t l_ = r + 2; std::swap(l, l_);
            return msg_str.substr(l_, r - l_);
        }
    }

    template <typename T>
    T convert_string(std::string const& str) {
        std::stringstream converter(str);
        T var; converter >> var;
        return var;
    }

    std::size_t findRightEdgeOfNextVar(std::string const& msg_str, std::size_t& l) {
        return msg_str.find(',', l);
    }

    template <typename First, typename... Rest>
    void Message::serializeWithArguments(std::string& req_str, First const& arg, Rest const&... args) const {
        add_command_type(req_str);
        add_first_class_variable(req_str, arg);
        for (auto const& arg : {args...}) {
            add_next_class_variable(req_str, arg);
        }
        complete_serialization(req_str);
    }
};


#endif