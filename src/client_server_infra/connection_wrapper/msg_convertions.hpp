#ifndef MSG_CONVERSION_HPP
#define MSG_CONVERSION_HPP

#include "zmq.hpp"

#include <commands/commands_module.hpp>

namespace lab5_7 {
    zmq::message_t to_msg(std::string const& str) {
        return zmq::message_t(str);
    }

    zmq::message_t to_msg(Command::cmd_ptr cmd) {
        return cmd->getMessage();
    }

    std::string to_string(zmq::message_t const& msg) {
        return msg.to_string();
    }
};


#endif