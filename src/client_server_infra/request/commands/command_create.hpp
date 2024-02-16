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
        CommandCreate(uint16_t newNodeId, uint16_t parentId) noexcept
             : newNodeId(newNodeId), parentId(parentId) {}

        virtual CommandType identify() const {
            return CommandType::Create;
        }

        static Command::cmd_ptr deserialize(std::string const& ser_cmd) {
            std::size_t pos = find_start_of_class_vars(ser_cmd);
            uint16_t newNodeId = getNextVar<uint16_t>(ser_cmd, pos);
            uint16_t parentId = getNextVar<uint16_t>(ser_cmd, pos);
            return std::make_shared<CommandCreate>(newNodeId, parentId);
        }
        
    protected:
        virtual void serialize_command(std::string& ser_cmd) const override final {
            serializeWithArguments(ser_cmd, newNodeId, parentId);
        }

        virtual void add_command_type(std::string& ser_cmd) const override final {
            ser_cmd += "Create";
        }

    };
};


#endif