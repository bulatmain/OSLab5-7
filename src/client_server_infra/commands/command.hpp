#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
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

        virtual void print() const {
            std::cout << this->serialize() << "\n"; 
        }

        virtual ~Command() = 0; 


    //protected:
    public:
        template <typename First, typename... Rest>
        std::string serializeWithArguments(First const& arg, Rest const&... args) const;

        virtual void add_class_type(std::string& ser_cmd) const = 0;
        template <typename T>
        void add_first_class_variable(std::string& ser_cmd, T const& var) const;
        template <typename T>
        void add_next_class_variable(std::string& ser_cmd, T const& var) const;
        void complete_serialization(std::string& ser_cmd) const;

        static std::size_t find_start_of_class_vars(std::string const& ser_cmd);    
        template <typename T>
        static T getNextVar(std::string const& ser_cmd, std::size_t& l);
        static std::string getNextVarString(std::string const& ser_cmd, std::size_t& l);
        template <typename T>
        static T convert_string(std::string const& str);
        static std::size_t findRightEdgeOfNextVar(std::string const& ser_cmd, std::size_t& l);

    };

    Command::~Command() {}
};



#endif