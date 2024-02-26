#ifndef RESP_SERIALIZATION_HPP
#define RESP_SERIALIZATION_HPP

#include "response.hpp"
#include "invalid_request/invalid_request.hpp"
#include "authorization_response/authorization_response.hpp"

namespace lab5_7 {
    ResponseType defineResponseType(std::string& resp_str) {
        std::string req_type = extractType(resp_str);
        if (req_type == "InvalidRequest") {
            return InvalidRequestEnum;
        } else if (req_type == "AuthorizationResponse") {
            return AuthorizationResponseEnum;
        } /*else if (req_type == "CommandResponse") {
            return CommandResponce;
        }*/ else {
            throw std::invalid_argument("Error, initiated at defineResponseType(...): can not define request type");
        }
    }

    Response::resp_ptr deserializeType(ResponseType type, std::string& resp_str) {
        if (type == InvalidRequestEnum) {
            return InvalidRequest::deserializeUnpacked(resp_str);
        } else if (type == AuthorizationResponseEnum) {
            return AuthorizationResponse::deserializeUnpacked(resp_str);
        } /*else if (type == CommandResponce) {
            return CommandResponce::deserializeUnpacked(resp_str);
        }*/ else {
            throw std::invalid_argument("Wtf?");
        }
    }

    Response::resp_ptr Response::deserializeUnpacked(std::string& resp_str) {
        ResponseType type = defineResponseType(resp_str);
        return deserializeType(type, resp_str);
    }

    Response::resp_ptr Response::deserialize(std::string& resp_str) {
        if (extractType(resp_str) != "Response") {
            throw std::invalid_argument("Error: attempt to deserialize invalid message string");
        }
        return deserializeUnpacked(resp_str);
    }

    Response::resp_ptr Response::deserialize(std::string&& resp_str) {
        auto _resp_str = std::move(resp_str);
        return std::move(Response::deserialize(_resp_str));
    }

    Response::resp_ptr Response::deserialize(std::string const& resp_str) {
        auto _resp_str = resp_str;
        return std::move(Response::deserialize(std::move(_resp_str)));
    }
};

#endif