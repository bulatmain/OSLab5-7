#ifndef REQ_SERIALIZATION_HPP
#define REQ_SERIALIZATION_HPP

#include "request_module.hpp"

#include <string>

namespace lab5_7 {
    RequestType defineRequestType(std::string& req_str) {
        std::string req_type = extractType(req_str);
        if (req_type == "Command") {
            return Command;
        } else if (req_type == "Authorization") {
            return Authorization;
        } else {            
            throw std::invalid_argument("Error, initiated at defineRequestType(...): can not define request type");
        }
    }

    Request::req_ptr deserializeType(RequestType type, std::string& req_str) {
        if (type == Command) {
            return Command::deserialize(std::move(req_str));
        } else if (type == Authorization) {
            return Authorization::deserialize(std::move(req_str));
        } else {
            throw std::invalid_argument("Wtf?");
        }
    }

    Request::req_ptr Request::deserialize(std::string& req_str) {
        RequestType type = defineRequestType(req_str);
        return deserializeType(type, req_str);
    }

    Request::req_ptr Request::deserialize(std::string&& req_str) {
        auto _req_str = std::move(req_str);
        return std::move(Request::deserialize(_req_str));
    }

    Request::req_ptr Request::deserialize(std::string const& req_str) {
        auto _req_str = req_str;
        return std::move(Request::deserialize(std::move(_req_str)));
    }

};

#endif