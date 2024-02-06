#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

#include <stdexcept>
#include <cassert>

#include "commands_module.hpp"

namespace lab5_7 {
    Command::cmd_ptr deserialize(std::string const& ser_cmd) {
        CommandType type = defineCommandType(ser_cmd);
        return std::move(deserializeType(type, ser_cmd));
    }

    static std::string serialize(Command const* cmd) {
        return cmd->serialize();
    }

    static std::string serialize(Command::const_cmd_ptr cmd) {
        return serialize(cmd.get());
    }

    static std::string serialize(Command::cmd_ptr cmd) {
        return serialize(cmd.get());
    }

    CommandType defineCommandType(std::string const& ser_cmd) {
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

    std::string extractCommandType(std::string const& ser_cmd) {
        std::size_t pos = ser_cmd.find('{');
        return ser_cmd.substr(0, pos);
    }

    Command::cmd_ptr deserializeType(CommandType type, std::string const& ser_cmd) {
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
};

#endif