#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H

#include <list>

namespace lab5_7 {
    enum CommandType {
        Create,
        Exec,
        PrintTree
    };

    static std::list<CommandType> commandTypeList{
        Create,
        Exec,
        PrintTree
    };

};

#endif