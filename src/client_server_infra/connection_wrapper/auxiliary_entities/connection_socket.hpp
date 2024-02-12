#ifndef ONE_SOCKET_CONNECTION_HPP
#define ONE_SOCKET_CONNECTION_HPP

#include "zmq_connection.hpp"
#include "exceptions.hpp"

#include "auxiliary_value_objects/value_objects.hpp"

namespace lab5_7 {
    // TODO:
    // Add recv and send zmq::result_t shared_ptr's
    // send and recv methods now are void
    // add method getResult() for send and recv methods
    // add wait method with time and contidion overloads. 
    class ConnectionSocket : public ZMQConnection {
    protected:
        zmq::socket_t socket;

        FlagVO is_connected;
        EndpointVO endpoint;

    public:
        ConnectionSocket(zmq::socket_type sock_type, std::string const& endpoint)
            : socket(context, sock_type), is_connected(false), endpoint(endpoint) {}
        
        ConnectionSocket(zmq::socket_type sock_type, std::string&& endpoint)
            : socket(context, sock_type), is_connected(false), endpoint(std::move(endpoint)) {}

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

        void changeEndpoint(std::string const& str) {
            endpoint.setValue(str);
        }

        void changeEndpoint(std::string&& str) {
            endpoint.setValue(std::move(str));
        }

    };


};

#endif