#ifndef RESPONCE_HPP
#define RESPONCE_HPP

#include "../message.hpp"
#include "response_types.hpp"

namespace lab5_7 {
    class Response : public Message {
    public:
        using resp_ptr = std::shared_ptr<Response>;

        static resp_ptr deserialize(std::string& req_str);
        static resp_ptr deserialize(std::string&& req_str);
        static resp_ptr deserialize(std::string const& req_str);
        static resp_ptr deserializeUnpacked(std::string& req_str);

        virtual ~Response() = default;

    protected:
        Response() = default;

        virtual void serialize_response(std::string& resp_str) const = 0;

        virtual void serialize_message(std::string& msg_str) const final {
            add_response_header(msg_str);
            serialize_response(msg_str);
            complete_serialization(msg_str);
        }

        void add_response_header(std::string& resp_str) const {
            resp_str += "Response{";
        }

    };

};

#endif