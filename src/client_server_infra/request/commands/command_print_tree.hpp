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

        static Command::cmd_ptr deserialize(std::string const& ser_cmd) {
            return std::make_shared<CommandPrintTree>();
        }

    protected:
        virtual void serialize_command(std::string& ser_cmd) const override final {
            add_command_type(ser_cmd);
            ser_cmd += "{}";
        } 

        virtual void add_command_type(std::string& ser_cmd) const override final {
            ser_cmd += "PrintTree";
        }
 
    };
};


#endif