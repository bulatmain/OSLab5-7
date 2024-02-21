#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../message.hpp"
#include "request_types.hpp"

namespace lab5_7 {
    class Request : public Message {
    public:
        using req_ptr = std::shared_ptr<Request>;

        static req_ptr deserialize(std::string& req_str);
        static req_ptr deserialize(std::string&& req_str);
        static req_ptr deserialize(std::string const& req_str);

        virtual ~Request() = 0;
    protected:
        virtual void serialize_request(std::string& req_str) const = 0;

        virtual void add_message_type(std::string& req_str) const final {
            req_str += "Request{";
        }
        virtual void add_request_type(std::string& req_str) const = 0;

    };

    Request::~Request() {};
};

#endif