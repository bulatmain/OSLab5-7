#ifndef AUTHORIZATION_WITH_NAME
#define AUTHORIZATION_WITH_NAME

#include "authorization.hpp"

namespace lab5_7 {
    class AuthWithName : public Authorization {
    public:
        std::string const name;

    public:
        AuthWithName(std::string const& endpoint, std::string const& name) noexcept
            : Authorization(endpoint), name(name) {}

        AuthWithName(std::string&& endpoint, std::string&& name) noexcept
            : Authorization(std::move(endpoint)), name(std::move(name)) {}
    
        static Authorization::auth_ptr deserealize(std::string const& auth_str) {
            std::size_t pos = find_start_of_class_vars(auth_str);
            std::string endpoint = getNextVar<std::string>(auth_str, pos);
        }

    protected:
        virtual void serialize_authorization(std::string& auth_str) const override final {
            serializeWithArguments(auth_str, endpoint, name);
        }

        virtual void add_authorization_type(std::string& auth_str) const override final {
            auth_str += "AuthWithName";
        }

    };
};

#endif