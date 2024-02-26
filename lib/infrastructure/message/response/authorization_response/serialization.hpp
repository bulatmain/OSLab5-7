#ifndef AUTH_RESP_SERIALIZATION_HPP
#define AUTH_RESP_SERIALIZATION_HPP

#include "authorization_response.hpp"
#include "authorization_accepted.hpp"
#include "authorization_denied.hpp"

namespace lab5_7 {
    AuthorizationResponseType defineAuthRespType(std::string& auth_str) {
        std::string req_type = extractType(auth_str);
        if (req_type == "AuthorizationAccepted") {
            return AuthorizationAcceptedEnum;
        } else if (req_type == "AuthorizationDenied") {
            return AuthorizationDeniedEnum;
        } else {
            throw std::invalid_argument("Error, initiated at defineAuthRespType(...): can not define request type");
        }
    }

    AuthorizationResponse::auth_resp_ptr deserializeType(AuthorizationResponseType type, std::string& auth_str) {
        if (type == AuthorizationAcceptedEnum) {
            return AuthorizationAccepted::deserializeUnpacked(auth_str);
        } else if (type == AuthorizationDeniedEnum) {
            return AuthorizationDenied::deserializeUnpacked(auth_str);
        } else {
            throw std::invalid_argument("Wtf?");
        }
    }

    AuthorizationResponse::auth_resp_ptr AuthorizationResponse::deserializeUnpacked(std::string& auth_str) {
        AuthorizationResponseType type = defineAuthRespType(auth_str);
        return deserializeType(type, auth_str);
    }

    AuthorizationResponse::auth_resp_ptr AuthorizationResponse::deserialize(std::string& auth_str) {
        if (extractType(auth_str) != "Response") {
            throw std::invalid_argument("Error: attempt to deserialize invalid message string");
        }
        return deserializeUnpacked(auth_str);
    }

    AuthorizationResponse::auth_resp_ptr AuthorizationResponse::deserialize(std::string&& auth_str) {
        auto _auth_str = std::move(auth_str);
        return std::move(AuthorizationResponse::deserialize(_auth_str));
    }

    AuthorizationResponse::auth_resp_ptr AuthorizationResponse::deserialize(std::string const& auth_str) {
        auto _auth_str = auth_str;
        return std::move(AuthorizationResponse::deserialize(std::move(_auth_str)));
    }
};

#endif