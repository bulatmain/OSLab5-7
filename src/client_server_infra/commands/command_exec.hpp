#ifndef COMMAND_EXEC_HPP
#define COMMAND_EXEC_HPP

#include <cinttypes>
#include <vector>

#include "command.hpp"

namespace lab5_7 {
    class CommandExec : public Command {
    public:
        uint16_t nodeId;
        std::vector<double> k;

    public:
        CommandExec(uint16_t nodeId, std::vector<double> k)
             : nodeId(nodeId), k(k) {}

        CommandExec(uint16_t nodeId, std::vector<double>&& k)
             : nodeId(nodeId), k(std::move(k)) {}

        CommandExec(uint16_t nodeId, std::initializer_list<double> ilist) 
            : nodeId(nodeId), k(ilist) {}

        template <typename... Args>
        CommandExec(uint16_t nodeId, Args... args) 
            : nodeId(nodeId), k({args...}) {}

        virtual CommandType identify() const {
            return CommandType::Exec;
        }

        virtual std::string serialize() const {
            return std::move(serializeWithArguments(nodeId, k.size(), ));
        } 

        void add_class_type(std::string& ser_cmd) const {
            ser_cmd += "Create";
        }

        virtual Command::cmd_ptr deserialize(std::string const& ser_cmd) const {
            ser_cmd.at(0);
            return std::make_shared<CommandCreate>(1, 2);
        }  

        virtual ~CommandExec() = default;
    };
};


#endif