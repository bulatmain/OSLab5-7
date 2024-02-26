#ifndef PUSH_CONNECTION_HPP
#define PUSH_CONNECTION_HPP

#include "connection_socket.hpp"

namespace lab5_7 {
    class PushConnection : public ConnectionSocket {
    public:
        PushConnection(std::string const& endpoint) 
            : ConnectionSocket(zmq::socket_type::push, endpoint) {}

        PushConnection(std::string&& endpoint) 
            : ConnectionSocket(zmq::socket_type::push, std::move(endpoint)) {}

        void push(zmq::message_t& msg) {
            if (!is_connected) {
                throw AttemptToSendMsgWithNotConnectedSocketException();
            } else {
                send(msg);
            }
        }

        void pushDontWait(zmq::message_t& msg) {
            if (!is_connected) {
                throw AttemptToSendMsgWithNotConnectedSocketException();
            } else {
                send(msg, zmq::send_flags::dontwait);
            }
        }

        void pushSendMore(zmq::message_t& msg) {
            if (!is_connected) {
                throw AttemptToSendMsgWithNotConnectedSocketException();
            } else {
                send(msg, zmq::send_flags::sndmore);
            }
        }

        send_res_ptr getPushResult() {
            return std::move(send_res);
        }

    };
};

#endif