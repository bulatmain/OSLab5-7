#ifndef MSG_QUEUE_EXCEPTIONS
#define MSG_QUEUE_EXCEPTIONS

#include <string>
#include <exception>

namespace lab5_7 {
    class MsgQueueException : public std::exception {
    protected:
        std::string msg;
    
    public:
        MsgQueueException(const char* c_str) noexcept
            : msg(c_str) {}
        MsgQueueException(std::exception const& e) noexcept 
            : MsgQueueException(e.what()) {}
        MsgQueueException(std::string const& str) noexcept 
            : MsgQueueException(str.c_str()) {}
        
        const char* what() noexcept {
            return msg.c_str();
        }

    };

    class AttemptToLaunchAlreadyRunningThread : public MsgQueueException {
    public:
        AttemptToLaunchAlreadyRunningThread() noexcept
            : MsgQueueException("Error: attemt to launch already running thread") {}
    };

    class AttemptToStopNotRunningThread : public MsgQueueException {
    public:
        AttemptToStopNotRunningThread() noexcept
            : MsgQueueException("Error: attemt to stop not running thread") {}
    };
};

#endif