#ifndef COMMAND_CREATE_HPP
#define COMMAND_CREATE_HPP

#include <cinttypes>

#include "command.hpp"

namespace lab5_7 {
    class CommandCreate : public Command {
    public:
        uint16_t const newNodeId;
        uint16_t const parentId; 
    public:
        CommandCreate(uint16_t newNodeId, uint16_t parentId) noexcept
             : newNodeId(newNodeId), parentId(parentId) {}

        virtual CommandType identifyCommand() const {
            return CommandType::CreateEnum;
        }

        static Command::cmd_ptr deserialize(std::string& ser_cmd) {
            if (extractType(ser_cmd) != "Create") {
                throw std::invalid_argument("Error: trying to deserialize invalid command_create string");
            }
            return deserializeUnpacked(ser_cmd);
        }

        static Command::cmd_ptr deserializeUnpacked(std::string const& ser_cmd) {
            std::size_t pos = find_start_of_class_vars(ser_cmd);
            uint16_t newNodeId = getNextVar<uint16_t>(ser_cmd, pos);
            uint16_t parentId = getNextVar<uint16_t>(ser_cmd, pos);
            return std::make_shared<CommandCreate>(newNodeId, parentId);
        }
        
    protected:
        virtual void serialize_command(std::string& ser_cmd) const override final {
            add_command_create_header(ser_cmd);
            add_first_class_variable(ser_cmd, newNodeId);
            add_next_class_variable(ser_cmd, parentId);
            complete_serialization(ser_cmd);
        }

        void add_command_create_header(std::string& ser_cmd) const {
            ser_cmd += "Create{";
        }

    };
};


#endif