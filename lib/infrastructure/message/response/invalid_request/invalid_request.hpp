#ifndef INVALID_REQUEST_HPP
#define INVALID_REQUEST_HPP

#include "../response.hpp"

namespace lab5_7 {
    class InvalidRequest : public Response {
    public:
        using ireq_ptr = std::shared_ptr<InvalidRequest>;

        static ireq_ptr deserialize(std::string& ireq_ser) {
            if (extractType(ireq_ser) != "InvalidRequest") {
                throw std::invalid_argument("Error: attempt to deserialize invalid string");
            }
            return deserializeUnpacked(ireq_ser);
        }
        static ireq_ptr deserialize(std::string&& ireq_ser) {
            auto _ireq_str = std::move(ireq_ser);
            return deserialize(ireq_ser);
        }
        static ireq_ptr deserialize(std::string const& ireq_ser) {
            auto _ireq_str = ireq_ser;
            return deserialize(ireq_ser);
        }

        static ireq_ptr deserializeUnpacked(std::string&) {
            return std::make_shared<InvalidRequest>();
        }

    protected:
        virtual void serialize_response(std::string& auth_str) const override final {
            add_invalid_request_header(auth_str);
            complete_serialization(auth_str);
        }

        void add_invalid_request_header(std::string& auth_str) const {
            auth_str += "InvalidRequest{";
        }

    };
};


#endif