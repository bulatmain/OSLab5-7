#ifndef MSG_TYPES_HPP
#define MSG_TYPES_HPP

#include <list>

namespace lab5_7 {
    enum MessageType {
        Request,
        Responce
    };

    static std::list<MessageType> const messageTypeList {
        Request,
        Responce        
    }

};

#endif