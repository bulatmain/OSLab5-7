#ifndef AUTHORIZATION_HPP
#define AUTHORIZATION_HPP

#include "../request/request.hpp"
#include "authorization_types.hpp"

namespace lab5_7 {
    class Authorization : public Request {
    public:
        std::string const endpoint;

    public:
        using auth_ptr = std::shared_ptr<Authorization>;

        static auth_ptr deserialize(std::string& ser_cmd);
        static auth_ptr deserialize(std::string&& ser_cmd);
        static auth_ptr deserialize(std::string const& ser_cmd);

        ~Authorization() = default;
    protected:             
    
        Authorization(std::string const& endpoint) noexcept
            : endpoint(endpoint) {} 

        Authorization(std::string&& endpoint) noexcept
            : endpoint(std::move(endpoint)) {} 
    
        virtual void serialize_request(std::string& auth_str) const override final {
            add_request_type(auth_str);
            serialize_authorization(auth_str);
            complete_serialization(auth_str);
        }

        virtual void serialize_authorization(std::string& auth_str) const = 0;  

        virtual void add_request_type(std::string& auth_str) const {
            auth_str += "Authorization{";
        }

        virtual void add_authorization_type(std::string& auth_str) const = 0;

    };
};


#endif