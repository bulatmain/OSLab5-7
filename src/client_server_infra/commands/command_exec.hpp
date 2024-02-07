#ifndef COMMAND_EXEC_HPP
#define COMMAND_EXEC_HPP

#include <cinttypes>
#include <iostream>
#include <vector>

#include "command.hpp"

namespace lab5_7 {
    class CommandExec : public Command {
    public:
        uint16_t nodeId;
        std::vector<double> k;

    public:
        CommandExec(uint16_t nodeId, std::vector<double> const& k)
             : nodeId(nodeId), k(k) {}

        CommandExec(uint16_t nodeId, std::vector<double>&& k)
             : nodeId(nodeId), k(std::move(k)) {}

        CommandExec(uint16_t nodeId, std::initializer_list<double> ilist) 
            : nodeId(nodeId), k(ilist) {}

        template <typename... Args>
        CommandExec(uint16_t nodeId, Args... args) 
            : nodeId(nodeId), k({args...}) {}

        virtual void print() const {
            std::cout << this->serialize() << "\n"; 
        }

        virtual CommandType identify() const {
            return CommandType::Exec;
        }

        virtual std::string serialize() const {
            std::string ser_cmd;
            add_class_type(ser_cmd);
            add_first_class_variable(ser_cmd, nodeId);
            add_next_class_variable(ser_cmd, k.size());
            for (auto const& k_i : k) {
                add_next_class_variable(ser_cmd, k_i);
            }
            complete_serialization(ser_cmd);
            return ser_cmd;
        } 

        virtual void add_class_type(std::string& ser_cmd) const {
            ser_cmd += "Exec";
        }

        static Command::cmd_ptr deserialize(std::string const& ser_cmd) {
            std::size_t pos = find_start_of_class_vars(ser_cmd);
            uint16_t nodeId = getNextVar<uint16_t>(ser_cmd, pos);
            uint64_t n = getNextVar<uint64_t>(ser_cmd, pos);
            std::vector<double> k(n);
            for (auto& k_i : k) {
                k_i = getNextVar<double>(ser_cmd, pos);
            }
            return std::make_shared<CommandExec>(nodeId, std::move(k));
        }  

        virtual ~CommandExec() = default;
    };
};


#endif