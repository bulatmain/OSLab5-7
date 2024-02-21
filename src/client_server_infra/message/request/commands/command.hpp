#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <concepts>

#include "../request.hpp"
#include "command_types.hpp"

namespace lab5_7 {
    class Command : public Request {
    public:
        using cmd_ptr = std::shared_ptr<Command>;

        virtual CommandType identify() const = 0;

        static cmd_ptr deserialize(std::string& ser_cmd);
        static cmd_ptr deserialize(std::string&& ser_cmd);
        static cmd_ptr deserialize(std::string const& ser_cmd);

        virtual void print() const {
            std::cout << this->serialize() << "\n"; 
        }

        virtual ~Command() = 0; 

    protected:
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

    };

    Command::~Command() {}
};



#endif