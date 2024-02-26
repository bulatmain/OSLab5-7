#ifndef REQUEST_TYPES_HPP
#define REQUEST_TYPES_HPP

#include <list>

namespace lab5_7 {
    enum RequestType {
        Command,
        Authorization
    };

    static const std::list<RequestType> requestTypeList{
        Command,
        Authorization
    };
};

#endif