#ifndef RESPONSE_TYPES_HPP
#define RESPONSE_TYPES_HPP

#include <list>

namespace lab5_7 {
    enum ResponseType {
        AuthorizationResponseEnum,
        InvalidRequestEnum,
        CommandResponceEnum
    };

    static const std::list<ResponseType> responceTypeList{
        AuthorizationResponseEnum,
        InvalidRequestEnum,
        CommandResponceEnum
    };
};

#endif