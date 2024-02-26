#ifndef AUTHORIZATION_RESPONSE_HPP
#define AUTHORIZATION_RESPONSE_HPP

#include "../response.hpp"
#include "authorization_response_types.hpp"

namespace lab5_7 {
    class AuthorizationResponse : public Response {
    public:
        using auth_resp_ptr = std::shared_ptr<AuthorizationResponse>;

        static auth_resp_ptr deserialize(std::string& ser_cmd);
        static auth_resp_ptr deserialize(std::string&& ser_cmd);
        static auth_resp_ptr deserialize(std::string const& ser_cmd);
        static auth_resp_ptr deserializeUnpacked(std::string& ser_cmd);

    protected:    
        virtual void serialize_response(std::string& resp_str) const override final {
            add_auth_response_header(resp_str);
            serialize_authorization_response(resp_str);
            complete_serialization(resp_str);
        }

        void add_auth_response_header(std::string& auth_str) const {
            auth_str += "AuthorizationResponse{";
        }

        virtual void serialize_authorization_response(std::string& auth_str) const = 0;  

    };
};


#endif