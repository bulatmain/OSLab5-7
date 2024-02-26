#ifndef AUTHORIZATION_WITH_NAME
#define AUTHORIZATION_WITH_NAME

#include "authorization_module.hpp"
#include "../serialization.hpp"

namespace lab5_7 {
    class AuthorizationWithName : public Authorization {
    public:
        std::string const name;

    public:
        AuthorizationWithName(std::string const& endpoint, std::string const& name) noexcept
            : Authorization(endpoint), name(name) {}

        AuthorizationWithName(std::string&& endpoint, std::string&& name) noexcept
            : Authorization(std::move(endpoint)), name(std::move(name)) {}
    
        static Authorization::auth_ptr deserialize(std::string& auth_str) {
            if (extractType(auth_str) != "AuthorizationWithName") {
                throw std::invalid_argument("Error: trying to deserialize invalid authorization_with_name string");
            }
            return deserializeUnpacked(auth_str);
        }

        static Authorization::auth_ptr deserialize(std::string&& auth_str) {
            auto _ireq_str = std::move(auth_str);
            return deserialize(auth_str);
        }
        static Authorization::auth_ptr deserialize(std::string const& auth_str) {
            auto _auth_str = auth_str;
            return deserialize(auth_str);
        }

        static Authorization::auth_ptr deserializeUnpacked(std::string const& auth_str) {
            std::size_t pos = find_start_of_class_vars(auth_str);
            std::string endpoint = getNextVar<std::string>(auth_str, pos);
            std::string name = getNextVar<std::string>(auth_str, pos);
            return std::make_shared<AuthorizationWithName>(endpoint, name);
        }

    protected:
        virtual void serialize_authorization(std::string& auth_str) const override final {
            add_auth_with_name_header(auth_str);
            add_first_class_variable(auth_str, endpoint);
            add_next_class_variable(auth_str, name);
            complete_serialization(auth_str);
        }

        void add_auth_with_name_header(std::string& auth_str) const {
            auth_str += "AuthorizationWithName{";
        }

    };
};

#endif