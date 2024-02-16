#ifndef CMD_SERIALIZATION_HPP
#define CMD_SERIALIZATION_HPP

#include <sstream>
#include <stdexcept>
#include <cassert>

#include "commands_module.hpp"

namespace lab5_7 {
    template <typename First, typename... Rest>
    void Command::serializeWithArguments(std::string& req_str, First const& arg, Rest const&... args) const {
        add_command_type(req_str);
        add_first_class_variable(req_str, arg);
        for (auto const& arg : {args...}) {
            add_next_class_variable(req_str, arg);
        }
        complete_serialization(req_str);
    }

    std::string extractCommandType(std::string& ser_cmd) {
        std::size_t pos = ser_cmd.find('{');
        auto command_type = ser_cmd.substr(0, pos); 
        ser_cmd = ser_cmd.substr(pos + 1, ser_cmd.size() - 1);
        return command_type;
    }

    CommandType defineCommandType(std::string& ser_cmd) {
        std::string command_type = extractCommandType(ser_cmd);
        if (command_type == "Create") {
            return CommandType::Create;
        } else if (command_type == "Exec") {
            return CommandType::Exec;
        } else if (command_type == "PrintTree") {
            return CommandType::PrintTree;
        } else {
            throw std::invalid_argument("Error, initiated at defineCommandType(...): can not define command type");
        }
    }

    Command::cmd_ptr deserializeType(CommandType type, std::string& ser_cmd) {
        switch (type) {
        case CommandType::Create:
            return std::move(CommandCreate::deserialize(ser_cmd));
        case CommandType::Exec:
            return std::move(CommandExec::deserialize(ser_cmd));
        case CommandType::PrintTree:
               return std::move(CommandPrintTree::deserialize(ser_cmd));
        default:
            assert(false);
        }
    }

    Command::cmd_ptr Command::deserialize(std::string& ser_cmd) {
        CommandType type = defineCommandType(ser_cmd);
        return std::move(deserializeType(type, ser_cmd));
    }

    Command::cmd_ptr Command::deserialize(std::string&& ser_cmd) {
        auto _ser_cmd = std::move(ser_cmd);
        return std::move(Command::deserialize(_ser_cmd));
    }

    Command::cmd_ptr Command::deserialize(std::string const& ser_cmd) {
        auto _ser_cmd = ser_cmd;
        return std::move(Command::deserialize(std::move(_ser_cmd)));
    }
};

#endif