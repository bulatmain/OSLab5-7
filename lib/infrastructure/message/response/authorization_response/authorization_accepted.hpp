#ifndef AUTHORIZATION_ACCEPTED_HPP
#define AUTHORIZATION_ACCEPTED_HPP

#include "authorization_response.hpp"

namespace lab5_7 {
    class AuthorizationAccepted : public AuthorizationResponse {
    public:
        using auth_acc_ptr = std::shared_ptr<AuthorizationAccepted>;

        static auth_acc_ptr deserialize(std::string& auth_ser) {
            if (extractType(auth_ser) != "AuthorizationAccepted") {
                throw std::invalid_argument("Error: attempt to deserialize invalid string");
            }
            return deserializeUnpacked(auth_ser);
        }
        static auth_acc_ptr deserialize(std::string&& auth_ser) {
            auto _auth_ser = std::move(auth_ser);
            return deserialize(auth_ser);
        }
        static auth_acc_ptr deserialize(std::string const& auth_ser) {
            auto _auth_ser = auth_ser;
            return deserialize(auth_ser);
        }

        static auth_acc_ptr deserializeUnpacked(std::string&) {
            return std::make_shared<AuthorizationAccepted>();
        }

    protected:
        virtual void serialize_authorization_response(std::string& auth_str) const override final {
            add_auth_accepted_header(auth_str);
            complete_serialization(auth_str);
        }

        void add_auth_accepted_header(std::string& auth_str) const {
            auth_str += "AuthorizationAccepted{";
        }

    };
};

#endif