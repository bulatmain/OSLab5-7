#ifndef RESPONSE_TYPES_HPP
#define RESPONSE_TYPES_HPP

#include <list>

namespace lab5_7 {
    enum ResponseType {
        AuthorizationResponseEnum,
        InvalidRequestEnum,
        BadCreateRespEnum,
        GoodCreateRespEnum,
        BadExecRespEnum,
        GoodExecRespEnum,
        NodeExistResponseEnum,
        NodeIsDeadResponseEnum,
        HeartbeatEnum
    };

    static const std::list<ResponseType> responceTypeList{
        AuthorizationResponseEnum,
        InvalidRequestEnum,
        BadCreateRespEnum,
        GoodCreateRespEnum,
        BadExecRespEnum,
        GoodExecRespEnum,
        NodeExistResponseEnum,
        NodeIsDeadResponseEnum,
        HeartbeatEnum
    };
};

#endif