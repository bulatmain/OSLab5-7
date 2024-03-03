#ifndef MSG_TYPES_HPP
#define MSG_TYPES_HPP

#include <list>

namespace lab5_7 {
    enum MessageType {
        RequestEnum,
        ResponseEnum,
        KillSignalEnum
    };

    static std::list<MessageType> const messageTypeList {
        RequestEnum,
        ResponseEnum,
        KillSignalEnum  
    };

};

#endif