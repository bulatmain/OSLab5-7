#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <memory>

#include "command_types.h"

namespace lab5_7 {
    class Command {
    public:
        using cmd_ptr = std::shared_ptr<Command>;
        using const_cmd_ptr = std::shared_ptr<Command const>;

        virtual CommandType identify() const = 0;

        // String format: <CommandType as Command, CommandCreate, etc.>{class_variable_1, class_variable_2, ...}
        virtual std::string serialize() const = 0; 
        virtual Command::cmd_ptr deserialize(std::string const& ser_cmd) const = 0;

        virtual ~Command() = 0; 

    protected:
        virtual void add_class_type(std::string& ser_cmd) const = 0;
        template <typename T>
        void add_first_class_variable(std::string& ser_cmd, T var) const {
            ser_cmd += "{" + std::to_string(var);
        }
        template <typename T>
        void add_next_class_variable(std::string& ser_cmd, T var) const {
            ser_cmd += ", " + std::to_string(var);
        }
        void complete_serialization(std::string& ser_cmd) const {
            ser_cmd += "}";
        }
        template <typename First, typename... Rest>
        std::string serializeWithArguments(First arg, Rest... args) const {
            std::string ser_cmd;
            add_class_type(ser_cmd);
            add_first_class_variable(ser_cmd, arg);
            for (auto const arg : {args...}) {
                add_next_class_variable(ser_cmd, arg);
            }
            complete_serialization(ser_cmd);
            return ser_cmd;
        }
    };

    Command::~Command() {}
};



#endif