#ifndef AUTHORIZATION_TYPES_HPP
#define AUTHORIZATION_TYPES_HPP

#include <list>

namespace lab5_7 {
    enum AuthorizationType {
        AuthWithName
    };

    static const std::list<AuthorizationType> requestTypeList{
        AuthWithName
    };
};

#endif