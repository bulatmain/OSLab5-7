#ifndef CONCURRENT_QUEUE_EXCEPTIONS_HPP
#define CONCURRENT_QUEUE_EXCEPTIONS_HPP

#include <string>
#include <exception>

namespace lab5_7 {
    class ConcurrentQueueException : public std::exception {
    protected:
        std::string msg;
    
    public:
        ConcurrentQueueException(const char* c_str) noexcept
            : msg(c_str) {}
        ConcurrentQueueException(std::exception const& e) noexcept 
            : ConcurrentQueueException(e.what()) {}
        ConcurrentQueueException(std::string const& str) noexcept 
            : ConcurrentQueueException(str.c_str()) {}
        
        const char* what() noexcept {
            return msg.c_str();
        }

    };

    class BadAttemtPopFromQueue : public ConcurrentQueueException {
    public:
        BadAttemtPopFromQueue() noexcept
            : ConcurrentQueueException("Error: bad attemt to pop from queue") {}
    }; 

    class BadAttemtPopFromQueueInGivenTime : public ConcurrentQueueException {
    public:
        BadAttemtPopFromQueueInGivenTime() noexcept
            : ConcurrentQueueException("Error: bad attemt to pop from queue in given time") {}
    };

};


#endif