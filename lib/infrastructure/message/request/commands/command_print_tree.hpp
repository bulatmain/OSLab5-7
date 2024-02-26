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

        static Command::cmd_ptr deserialize(std::string& ser_cmd) {
            if (extractType(ser_cmd) != "PrintTree") {
                throw std::invalid_argument("Error: trying to deserialize invalid command_print_tree string");
            }
            return deserializeUnpacked(ser_cmd);
        }

        static Command::cmd_ptr deserializeUnpacked(std::string const&) {
            return std::make_shared<CommandPrintTree>();
        }

    protected:
        virtual void serialize_command(std::string& ser_cmd) const override final {
            add_command_print_tree_header(ser_cmd);
            complete_serialization(ser_cmd);
        } 

        void add_command_print_tree_header(std::string& ser_cmd) const {
            ser_cmd += "PrintTree{";
        }
 
    };
};


#endif