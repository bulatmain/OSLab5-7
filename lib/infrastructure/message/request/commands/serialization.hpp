#ifndef CMD_SERIALIZATION_HPP
#define CMD_SERIALIZATION_HPP

#include "command.hpp"
#include "command_create.hpp"
#include "command_exec.hpp"
#include "command_print_tree.hpp"
#include "command_pass.hpp"

namespace lab5_7 {
    CommandType defineCommandType(std::string& ser_cmd) {
        std::string command_type = extractType(ser_cmd);
        if (command_type == "Create") {
            return CommandType::CreateEnum;
        } else if (command_type == "Exec") {
            return CommandType::ExecEnum;
        } else if (command_type == "PrintTree") {
            return CommandType::PrintTreeEnum;
        } else if (command_type == "Pass") {
            return CommandType::PassEnum;
        } else {
            throw std::invalid_argument("Error, initiated at defineCommandType(...): can not define command type");
        }
    }

    Command::cmd_ptr deserializeType(CommandType type, std::string& ser_cmd) {
        switch (type) {
        case CommandType::CreateEnum:
            return std::move(CommandCreate::deserializeUnpacked(ser_cmd));
        case CommandType::ExecEnum:
            return std::move(CommandExec::deserializeUnpacked(ser_cmd));
        case CommandType::PrintTreeEnum:
               return std::move(CommandPrintTree::deserializeUnpacked(ser_cmd));
        case CommandType::PassEnum:
               return std::move(CommandPass::deserializeUnpacked(ser_cmd));
        default:
            throw std::runtime_error("Wtf?");
        }
    }

    Command::cmd_ptr Command::deserializeUnpacked(std::string& ser_cmd) {
        CommandType type = defineCommandType(ser_cmd);
        return std::move(deserializeType(type, ser_cmd));
    }

    
    Command::cmd_ptr Command::deserialize(std::string& ser_cmd) {
        if (extractType(ser_cmd) != "Command") {
            throw std::invalid_argument("Error: trying to deserialize invalid command string");
        }
        return deserializeUnpacked(ser_cmd);
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