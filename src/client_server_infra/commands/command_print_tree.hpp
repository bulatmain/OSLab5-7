#ifndef COMMAND_PRINT_TREE_HPP
#define COMMAND_PRINT_TREE_HPP

#include <cinttypes>

#include "command.hpp"

namespace lab5_7 {
    class CommandPrintTree : public Command {
    public:
        virtual CommandType identify() const {
            return CommandType::PrintTree;
        }

        virtual std::string serialize() const {
            std::string ser_cmd;
            add_class_type(ser_cmd);
            return ser_cmd + "{}";
        } 

        virtual void add_class_type(std::string& ser_cmd) const {
            ser_cmd += "PrintTree";
        }

        static Command::cmd_ptr deserialize(std::string const& ser_cmd) {
            return std::make_shared<CommandPrintTree>();
        }  

        virtual ~CommandPrintTree() = default;
    };
};


#endif