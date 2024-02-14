#ifndef PULL_CONNECTION_HPP
#define PULL_CONNECTION_HPP

#include "bind_socket.hpp"

namespace lab5_7 {
    class PullConnection : public BindSocket {
    public:
        PullConnection(std::string const& endpoint) 
            : BindSocket(zmq::socket_type::pull, endpoint) {}

        PullConnection(std::string&& endpoint) 
            : BindSocket(zmq::socket_type::pull, std::move(endpoint)) {}

        void pull(zmq::message_t& msg) {
            if (!is_binded) {
                throw AttemptToRecvMsgWithNotBindedSocketException();
            } else {
                recv(msg); 
            }
        }

        void pullDontWait(zmq::message_t& msg) {
            if (!is_binded) {
                throw AttemptToRecvMsgWithNotBindedSocketException();
            } else {
                recv(msg, zmq::recv_flags::dontwait);
            }
        }

        recv_res_ptr getPullResult() {
            return std::move(recv_res);
        }

    };
};

#endif