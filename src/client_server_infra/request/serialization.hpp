#ifndef REQ_SERIALIZATION_HPP
#define REQ_SERIALIZATION_HPP

#include "request.hpp"
#include "commands/commands_module.hpp"

#include <string>

namespace lab5_7 {
    std::string serialize(Request::req_ptr req) {
        return req->serialize();
    }

    std::string extractRequestType(std::string& req_str) {
        auto l = req_str.find('{', 0);
        auto r = req_str.find('{', l + 1);
        auto req_type = req_str.substr(l + 1, r - l - 1);
        req_str = req_str.substr(r + 1, req_str.size() - 1);
        return req_type;
    }

    RequestType defineRequestType(std::string& req_str) {
        std::string req_type = extractRequestType(req_str);
        if (req_type == "Command") {
            return Command;
        } else {            
            throw std::invalid_argument("Error, initiated at defineRequestType(...): can not define request type");
        }
    }

    Request::req_ptr deserializeType(RequestType type, std::string& req_str) {
        if (type == Command) {
            return Command::deserialize(std::move(req_str));
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

    template <typename T>
    void Request::add_first_class_variable(std::string& req_str, T const& var) const {
        req_str += "{" + std::to_string(var);
    }
    template <typename T>
    void Request::add_next_class_variable(std::string& req_str, T const& var) const {
        req_str += ", " + std::to_string(var);
    }
    void Request::complete_serialization(std::string& req_str) const {
        req_str += "}";
    }

    std::size_t Request::find_start_of_class_vars(std::string const& req_str) {
        return req_str.find('{') + 1;
    }

    template <typename T>
    T Request::getNextVar(std::string const& req_str, std::size_t& l) {
        std::string var_str = getNextVarString(req_str, l);
        return convert_string<T>(var_str);
    }

    std::string Request::getNextVarString(std::string const& req_str, std::size_t& l) {
        std::size_t r = findRightEdgeOfNextVar(req_str, l);
        if (r == std::string::npos) {
            std::size_t l_ = req_str.size(); std::swap(l, l_);
            return req_str.substr(l_, req_str.size() - l_);
        } else {
            std::size_t l_ = r + 2; std::swap(l, l_);
            return req_str.substr(l_, r - l_);
        }
    }

    template <typename T>
    T Request::convert_string(std::string const& str) {
        std::stringstream converter(str);
        T var; converter >> var;
        return var;
    }

    std::size_t Request::findRightEdgeOfNextVar(std::string const& req_str, std::size_t& l) {
        return req_str.find(',', l);
    }
};

#endif