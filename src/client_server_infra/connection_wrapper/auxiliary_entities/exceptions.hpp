#ifndef CONNECTION_EXCEPTIONS
#define CONNECTION_EXCEPTIONS

#include <exception>
#include <string>

namespace lab5_7 {
    class ConnectionException : public std::exception {
    protected:
        std::string msg;
    
    public:
        ConnectionException(const char* c_str) noexcept
            : msg(c_str) {}
        ConnectionException(std::exception const& e) noexcept 
            : ConnectionException(e.what()) {}
        ConnectionException(std::string const& str) noexcept 
            : ConnectionException(str.c_str()) {}
        
        const char* what() noexcept {
            return msg.c_str();
        }

    };

    class AttemptToConnectAlreadyConnectedSocketException : public ConnectionException {
    public:
        AttemptToConnectAlreadyConnectedSocketException() noexcept
            : ConnectionException("Error: attempt to connect already connected socket") {}
    };

    class AttemptToBindAlreadyConnectedSocketException : public ConnectionException {
    public:
        AttemptToBindAlreadyConnectedSocketException() noexcept
            : ConnectionException("Error: attempt to bind already connected socket") {}
    };

    class AttemptToDisconnectNotConnectedSocketException : public ConnectionException {
    public:
        AttemptToDisconnectNotConnectedSocketException() noexcept
            : ConnectionException("Error: attempt to disconnect not connected socket") {}
    };

    class AttemptToSendMsgWithNotConnectedSocketException : public ConnectionException {
    public:
        AttemptToSendMsgWithNotConnectedSocketException() noexcept
            : ConnectionException("Error: attempt to send message with not connected socket") {}
    };

    class UnableToSendMsgInNonBlockingModeException : public ConnectionException {
    public:
        UnableToSendMsgInNonBlockingModeException() noexcept 
            : ConnectionException("Error: unable to send message in non blocking node") {}
    };


};

#endif