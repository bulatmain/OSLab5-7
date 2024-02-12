#ifndef CONNECTION_WRAPPER_VALUE_OBJECTS
#define CONNECTION_WRAPPER_VALUE_OBJECTS

#include <mutex>
#include <string>

namespace lab5_7 {
    class FlagVO {
    private:
        bool value;
        std::mutex mtx;
    public:
        FlagVO(bool value) : value(value) {}
        FlagVO(FlagVO const& other)
            : FlagVO(other.value) {}
        
        void setValue(bool value) {
            std::lock_guard<std::mutex> lock(mtx);
            this->value = value;
        }

        bool getValue() const {
            return value;
        }

        bool getValue() {
            std::lock_guard<std::mutex> lock(mtx);
            return const_cast<FlagVO const*>(this)->getValue();
        }
        
        operator bool() const {
            return getValue();
        }
    };


    class EndpointVO {
    private:
        std::string endpoint;
        std::mutex mtx;

    public:
        EndpointVO(std::string const& endpoint)
            : endpoint(endpoint) {}
        
        EndpointVO(std::string&& endpoint)
            : endpoint(std::move(endpoint)) {}

        EndpointVO(EndpointVO const& other)
            : EndpointVO(other.endpoint) {}

        EndpointVO(EndpointVO&& other)
            : EndpointVO(std::move(other.endpoint)) {}

        EndpointVO& operator=(EndpointVO const& other) {
            this->endpoint = other.endpoint;
        }

        EndpointVO& operator=(EndpointVO&& other) {
            this->endpoint = std::move(endpoint);
        }

        void setValue(std::string const& endpoint) {
            std::lock_guard<std::mutex> lock(mtx);
            this->endpoint = endpoint;
        }

        void setValue(std::string&& endpoint) {
            std::lock_guard<std::mutex> lock(mtx);
            this->endpoint = std::move(endpoint);
        }

        std::string const& getValue() const {
            return endpoint;
        }

        std::string const& getValue() {
            std::lock_guard<std::mutex> lock(mtx);
            return const_cast<EndpointVO const*>(this)->getValue();
        }

    };
};

#endif