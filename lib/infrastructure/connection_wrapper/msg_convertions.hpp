#ifndef MSG_CONVERSION_HPP
#define MSG_CONVERSION_HPP

#include "zmq.hpp"

namespace lab5_7 {
    zmq::message_t to_msg(std::string const& str) {
        return zmq::message_t(str);
    }

    std::string to_string(zmq::message_t const& msg) {
        return msg.to_string();
    }
};


#endif