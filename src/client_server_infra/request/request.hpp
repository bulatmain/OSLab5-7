#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "request_types.hpp"

#include <memory>
#include <string>

namespace lab5_7 {
    class Request {
    public:
        using req_ptr = std::shared_ptr<Request>;

        // String format: Request{<Derived_1>{<Derived_2>{...Derived_n{...}...}}}
        virtual std::string serialize() const final {
            std::string req_str;
            add_request_header(req_str);
            serialize_request(req_str);
            complete_serialization(req_str);
            return req_str;
        }

        static req_ptr deserialize(std::string& req_str);
        static req_ptr deserialize(std::string&& req_str);

        virtual ~Request() = 0;
    protected:
        virtual void serialize_request(std::string& req_str) const = 0;

        virtual void add_request_header(std::string& req_str) const final {
            req_str += "Request{";
        }
        virtual void add_request_type(std::string& req_str) const = 0;

        // Implementation in serialization.hpp
        template <typename T>
        void add_first_class_variable(std::string& req_str, T const& var) const;
        template <typename T>
        void add_next_class_variable(std::string& req_str, T const& var) const;
        void complete_serialization(std::string& req_str) const;

        static std::size_t find_start_of_class_vars(std::string const& req_str);    
        template <typename T>
        static T getNextVar(std::string const& req_str, std::size_t& l);
        static std::string getNextVarString(std::string const& req_str, std::size_t& l);
        template <typename T>
        static T convert_string(std::string const& str);
        static std::size_t findRightEdgeOfNextVar(std::string const& req_str, std::size_t& l);

    };

    Request::~Request() {}
};

#endif