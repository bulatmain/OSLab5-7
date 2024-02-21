#ifndef CMD_SERIALIZATION_HPP
#define CMD_SERIALIZATION_HPP

#include <stdexcept>

#include "commands_module.hpp"

namespace lab5_7 {
    CommandType defineCommandType(std::string& ser_cmd) {
        std::string command_type = extractType(ser_cmd);
        if (command_type == "Create") {
            return CommandType::Create;
        } else if (command_type == "Exec") {
            return CommandType::Exec;
        } else if (command_type == "PrintTree") {
            return CommandType::PrintTree;
        } else if (command_type == "Pass") {
            return CommandType::Pass;
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
        case CommandType::Pass:
               return std::move(CommandPass::deserialize(ser_cmd));
        default:
            throw std::runtime_error("Wtf?");
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