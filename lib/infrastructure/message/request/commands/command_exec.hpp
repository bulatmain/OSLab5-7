#ifndef COMMAND_EXEC_HPP
#define COMMAND_EXEC_HPP

#include <cinttypes>
#include <vector>

#include "command.hpp"

template <typename T>
concept ConvertableToDouble = std::is_convertible<T, double>::value;

namespace lab5_7 {
    class CommandExec : public Command {
    public:
        uint16_t const nodeId;
        std::vector<double> const k;

    public:
        CommandExec(uint16_t nodeId, std::vector<double> const& k)
             : nodeId(nodeId), k(k) {}

        CommandExec(uint16_t nodeId, std::vector<double>&& k)
             : nodeId(nodeId), k(std::move(k)) {}

        template <typename First, typename... Args>
        CommandExec(First arg, Args... args) 
            : nodeId(static_cast<uint16_t>(arg)), k({args...}) {}

        virtual CommandType identifyCommand() const {
            return CommandType::ExecEnum;
        }

        static Command::cmd_ptr deserialize(std::string& ser_cmd) {
            if (extractType(ser_cmd) != "Exec") {
                throw std::invalid_argument("Error: trying to deserialize invalid command_exec string");
            }
            return deserializeUnpacked(ser_cmd);
        }

        static Command::cmd_ptr deserializeUnpacked(std::string const& ser_cmd) {
            std::size_t pos = find_start_of_class_vars(ser_cmd);
            uint16_t nodeId = getNextVar<uint16_t>(ser_cmd, pos);
            uint64_t n = getNextVar<uint64_t>(ser_cmd, pos);
            std::vector<double> k(n);
            for (auto& k_i : k) {
                k_i = getNextVar<double>(ser_cmd, pos);
            }
            return std::make_shared<CommandExec>(nodeId, std::move(k));
        }  

    protected:
        virtual void serialize_command(std::string& ser_cmd) const override final {
            add_command_exec_header(ser_cmd);
            add_first_class_variable(ser_cmd, nodeId);
            add_next_class_variable(ser_cmd, k.size());
            for (auto const& k_i : k) {
                add_next_class_variable(ser_cmd, k_i);
            }
            complete_serialization(ser_cmd);
        } 

        void add_command_exec_header(std::string& ser_cmd) const {
            ser_cmd += "Exec{";
        }

    };
};


#endif