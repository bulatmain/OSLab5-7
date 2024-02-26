#ifndef AUTHORIZATION_RESPONSE_TYPES_HPP
#define AUTHORIZATION_RESPONSE_TYPES_HPP
#include <list>

namespace lab5_7 {
    enum AuthorizationResponseType {
        AuthorizationAcceptedEnum,
        AuthorizationDeniedEnum
    };

    static const std::list<AuthorizationResponseType> authorizationResponseTypeList{
        AuthorizationAcceptedEnum,
        AuthorizationDeniedEnum
    };
};


#endif