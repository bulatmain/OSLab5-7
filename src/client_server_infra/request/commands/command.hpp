#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <concepts>
#include <iostream>
#include <string>
#include <memory>

#include "../request.hpp"
#include "command_types.hpp"

namespace lab5_7 {
    class Command : public Request {
    public:
        using cmd_ptr = std::shared_ptr<Command>;
        using const_cmd_ptr = std::shared_ptr<Command const>;

        template <typename T, typename... Args>    
        static cmd_ptr construct(Args... args) {
            static_assert(std::is_base_of<Command, T>::value,
                "T must be a derived class of Base in Context<T>.");
            return std::make_shared<T>(args...);
        }

        virtual CommandType identify() const = 0;

        static cmd_ptr deserialize(std::string& ser_cmd);
        static cmd_ptr deserialize(std::string&& ser_cmd);
        static cmd_ptr deserialize(std::string const& ser_cmd);

        virtual void print() const {
            std::cout << this->serialize() << "\n"; 
        }

        virtual ~Command() = default; 

    protected:
        // String format: Command{CommandDerived_1{CommandDerived_2{...{class_variable_1, class_variable_2, ...}...}}}
        virtual void serialize_request(std::string& req_str) const override final {
            add_request_type(req_str);
            serialize_command(req_str);
            complete_serialization(req_str);
        }

        virtual void serialize_command(std::string& req_str) const = 0;        

        void add_request_type(std::string& req_str) const {
            req_str += "Command{";
        }

        virtual void add_command_type(std::string& req_str) const = 0;

        template <typename First, typename... Rest>
        void serializeWithArguments(std::string& req_str, First const& arg, Rest const&... args) const;

    };
};



#endif