#ifndef COMMAND_CREATE_HPP
#define COMMAND_CREATE_HPP

#include <cinttypes>

#include "command.hpp"

namespace lab5_7 {
    class CommandCreate : public Command {
    public:
        uint16_t newNodeId;
        uint16_t parentId; 
    public:
        CommandCreate(uint16_t newNodeId, uint16_t parentId)
             : newNodeId(newNodeId), parentId(parentId) {}

        virtual CommandType identify() const {
            return CommandType::Create;
        }

        virtual std::string serialize() const {
            return std::move(serializeWithArguments(newNodeId, parentId));
        } 

        void add_class_type(std::string& ser_cmd) const {
            ser_cmd += "Create";
        }

        virtual Command::cmd_ptr deserialize(std::string const& ser_cmd) const {
            ser_cmd.at(0);
            return std::make_shared<CommandCreate>(1, 2);
        }  

        virtual ~CommandCreate() = default;
    };
};


#endif