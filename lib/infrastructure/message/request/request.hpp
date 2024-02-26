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
        static req_ptr deserializeUnpacked(std::string& req_str);

        virtual ~Request() = default;
    protected:
        Request() = default;

        virtual void serialize_request(std::string& req_str) const = 0;

        virtual void serialize_message(std::string& msg_str) const final {
            add_request_header(msg_str);
            serialize_request(msg_str);
            complete_serialization(msg_str);
        }

        void add_request_header(std::string& req_str) const {
            req_str += "Request{";
        }

    };

};

#endif