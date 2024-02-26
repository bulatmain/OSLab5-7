#ifndef REQUEST_TYPES_HPP
#define REQUEST_TYPES_HPP

#include <list>

namespace lab5_7 {
    enum RequestType {
        CommandEnum,
        AuthorizationEnum
    };

    static const std::list<RequestType> requestTypeList{
        CommandEnum,
        AuthorizationEnum
    };
};

#endif