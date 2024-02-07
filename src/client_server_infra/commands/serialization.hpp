#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

#include <sstream>
#include <stdexcept>
#include <cassert>

#include "commands_module.hpp"

namespace lab5_7 {
    static std::string serialize(Command const* cmd) {
        return cmd->serialize();
    }

    static std::string serialize(Command::const_cmd_ptr cmd) {
        return serialize(cmd.get());
    }

    static std::string serialize(Command::cmd_ptr cmd) {
        return serialize(cmd.get());
    }

    std::string extractCommandType(std::string const& ser_cmd) {
        std::size_t pos = ser_cmd.find('{');
        return ser_cmd.substr(0, pos);
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

    Command::cmd_ptr deserialize(std::string const& ser_cmd) {
        CommandType type = defineCommandType(ser_cmd);
        return std::move(deserializeType(type, ser_cmd));
    }

    template <typename T>
    void Command::add_first_class_variable(std::string& ser_cmd, T const& var) const {
        ser_cmd += "{" + std::to_string(var);
    }
    template <typename T>
    void Command::add_next_class_variable(std::string& ser_cmd, T const& var) const {
        ser_cmd += ", " + std::to_string(var);
    }
    void Command::complete_serialization(std::string& ser_cmd) const {
        ser_cmd += "}";
    }

    template <typename First, typename... Rest>
    std::string Command::serializeWithArguments(First const& arg, Rest const&... args) const {
        std::string ser_cmd;
        add_class_type(ser_cmd);
        add_first_class_variable(ser_cmd, arg);
        for (auto const& arg : {args...}) {
            add_next_class_variable(ser_cmd, arg);
        }
        complete_serialization(ser_cmd);
        return ser_cmd;
    }

    std::size_t Command::find_start_of_class_vars(std::string const& ser_cmd) {
        return ser_cmd.find('{') + 1;
    }

    template <typename T>
    T Command::getNextVar(std::string const& ser_cmd, std::size_t& l) {
        std::string var_str = getNextVarString(ser_cmd, l);
        return convert_string<T>(var_str);
    }

    std::string Command::getNextVarString(std::string const& ser_cmd, std::size_t& l) {
        std::size_t r = findRightEdgeOfNextVar(ser_cmd, l);
        if (r == std::string::npos) {
            std::size_t l_ = ser_cmd.size(); std::swap(l, l_);
            return ser_cmd.substr(l_, ser_cmd.size() - l_);
        } else {
            std::size_t l_ = r + 2; std::swap(l, l_);
            return ser_cmd.substr(l_, r - l_);
        }
    }

    template <typename T>
    T Command::convert_string(std::string const& str) {
        std::stringstream converter(str);
        T var; converter >> var;
        return var;
    }

    std::size_t Command::findRightEdgeOfNextVar(std::string const& ser_cmd, std::size_t& l) {
        return ser_cmd.find(',', l);
    }
};

#endif