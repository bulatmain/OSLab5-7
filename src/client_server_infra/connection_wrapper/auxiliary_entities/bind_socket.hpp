#ifndef BIND_SOCKET_HPP
#define BIND_SOCKET_HPP

#include "one_socket_connection.hpp"

namespace lab5_7 {
    class BindSocket : public OneSocketConnection {
    protected:
        FlagVO is_binded;

    public:
        BindSocket(zmq::socket_type sock_type, std::string const& endpoint)
            : OneSocketConnection(sock_type, endpoint), is_binded(false) {}
        
        BindSocket(zmq::socket_type sock_type, std::string&& endpoint)
            : OneSocketConnection(sock_type, std::move(endpoint)), is_binded(false) {}

        void bind() {
            if (is_binded) {
                throw AttemptToBindAlreadyConnectedSocketException();
            } try {
                socket.bind(endpoint.getValue());
            } catch (zmq::error_t const& e) {
                throw ConnectionException(e);
            }
            is_binded.setValue(true);
        }

        void unbind() {
            if (!is_binded) {
                throw AttemptToUnbindNotConnectedSocketException();
            } try {
                socket.unbind(endpoint.getValue());
            } catch (zmq::error_t const& e) {
                throw ConnectionException(e);
            }
            is_binded.setValue(false);
        }
    };
};

#endif