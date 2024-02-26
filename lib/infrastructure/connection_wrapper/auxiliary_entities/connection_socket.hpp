#ifndef CONNECTION_SOCKET_HPP
#define CONNECTION_SOCKET_HPP

#include "one_socket_connection.hpp"

namespace lab5_7 {
    class ConnectionSocket : public OneSocketConnection {
    protected:
        FlagVO is_connected;

    public:
        ConnectionSocket(zmq::socket_type sock_type, std::string const& endpoint)
            : OneSocketConnection(sock_type, endpoint), is_connected(false) {}
        
        ConnectionSocket(zmq::socket_type sock_type, std::string&& endpoint)
            : OneSocketConnection(sock_type, std::move(endpoint)), is_connected(false) {}

        void connect() {
            if (is_connected) {
                throw AttemptToConnectAlreadyConnectedSocketException();
            } try {
                socket.connect(endpoint.getValue());
            } catch (zmq::error_t const& e) {
                throw ConnectionException(e);
            }
            is_connected.setValue(true);
        }

        void disconnect() {
            if (!is_connected) {
                throw AttemptToDisconnectNotConnectedSocketException();
            } try {
                socket.disconnect(endpoint.getValue());
            } catch (zmq::error_t const& e) {
                throw ConnectionException(e);
            }
            is_connected.setValue(false);
        }
    };
};

#endif