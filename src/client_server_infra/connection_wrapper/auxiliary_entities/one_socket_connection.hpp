#ifndef ONE_SOCKET_CONNECTION_HPP
#define ONE_SOCKET_CONNECTION_HPP

#include "zmq_connection.hpp"
#include "connection_exceptions.hpp"

#include "auxiliary_value_objects/value_objects.hpp"

namespace lab5_7 {
    class OneSocketConnection : public ZMQConnection {
    protected:
        using recv_res_ptr = std::shared_ptr<zmq::recv_result_t>;
        using send_res_ptr = std::shared_ptr<zmq::send_result_t>;
        zmq::socket_t socket;
        recv_res_ptr recv_res;
        send_res_ptr send_res;

        EndpointVO endpoint;

    protected:
        OneSocketConnection(zmq::socket_type sock_type, std::string const& endpoint)
            : socket(context, sock_type), endpoint(endpoint) {}
        
        OneSocketConnection(zmq::socket_type sock_type, std::string&& endpoint)
            : socket(context, sock_type), endpoint(std::move(endpoint)) {}

    public:
        void changeEndpoint(std::string const& str) {
            endpoint.setValue(str);
        }

        void changeEndpoint(std::string&& str) {
            endpoint.setValue(std::move(str));
        }
    
    protected:
        virtual void recv(zmq::message_t& msg, zmq::recv_flags flag = zmq::recv_flags::none) try {
            recv_res = std::make_shared<zmq::recv_result_t>(socket.recv(msg, flag));
        } catch (zmq::error_t const& e) {
            throw ConnectionException(e);
        }

        virtual void send(zmq::message_t& msg, zmq::send_flags flag = zmq::send_flags::none) try {
            send_res = std::make_shared<zmq::send_result_t>(socket.send(msg, flag));
        } catch (zmq::error_t const& e) {
            throw ConnectionException(e);
        }

    };


};

#endif