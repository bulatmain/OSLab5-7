#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H

#include <list>

namespace lab5_7 {
    enum CommandType {
        Create,
        Exec,
        PrintTree,
        Pass
    };

    static std::list<CommandType> const commandTypeList{
        Create,
        Exec,
        PrintTree,
        Pass
    };

    static std::list<CommandType> const clientSideCommandTypeList{
        Pass
    };

};

#endif