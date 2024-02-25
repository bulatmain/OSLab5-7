#ifndef AUTH_SERIALIZATION_HPP
#define AUTH_SERIALIZATION_HPP

#include "authorization.hpp"
#include "authorization_with_name.hpp"

#include <iostream>

namespace lab5_7 {
        AuthorizationType defineAuthorizeType(std::string& auth_ser) {
        std::string auth_type = extractType(auth_ser);
        if (auth_type == "AuthorizationWithName") {
            return AuthorizationType::AuthWithName;
        } else {
            throw std::invalid_argument("Error, initiated at defineAuthorizeType(...): can not define command type");
        }
    }

    Authorization::auth_ptr deserializeType(AuthorizationType type, std::string& auth_ser) {
        switch (type) {
        case AuthorizationType::AuthWithName:
            return std::move(AuthorizationWithName::deserializeUnpacked(auth_ser));
        default:
            throw std::runtime_error("Wtf?");
        }
    }

    Authorization::auth_ptr Authorization::deserializeUnpacked(std::string& auth_ser) {
        AuthorizationType type = defineAuthorizeType(auth_ser);
        return std::move(deserializeType(type, auth_ser));
    }

    Authorization::auth_ptr Authorization::deserialize(std::string& auth_ser) {
        if (extractType(auth_ser) != "Authorization") {
            throw std::invalid_argument("Error: trying to deserialize invalid authorization string");
        }
        return deserializeUnpacked(auth_ser);
    }

    Authorization::auth_ptr Authorization::deserialize(std::string&& auth_ser) {
        auto _auth_ser = std::move(auth_ser);
        return std::move(Authorization::deserialize(_auth_ser));
    }

    Authorization::auth_ptr Authorization::deserialize(std::string const& auth_ser)  {
        auto _auth_ser = auth_ser;
        return std::move(Authorization::deserialize(std::move(_auth_ser)));
    }

};

#endif