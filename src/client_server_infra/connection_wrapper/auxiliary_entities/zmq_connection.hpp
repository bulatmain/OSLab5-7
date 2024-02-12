#ifndef AUX_ENTITIES_HPP
#define AUX_ENTITIES_HPP

#include "zmq.hpp"

#include <map>

namespace lab5_7 {
    class ZMQConnection {
    protected:
        zmq::context_t context;
        ZMQConnection() = default;

    };

};

#endif