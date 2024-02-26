#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H

#include <list>

namespace lab5_7 {
    enum CommandType {
        CreateEnum,
        ExecEnum,
        PrintTreeEnum,
        PassEnum
    };

    static std::list<CommandType> const commandTypeList{
        CreateEnum,
        ExecEnum,
        PrintTreeEnum,
        PassEnum
    };

    static std::list<CommandType> const clientSideCommandTypeList{
        PassEnum
    };

};

#endif