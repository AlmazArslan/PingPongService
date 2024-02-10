#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <map>
#include <string>
#include "connection.h"

using callbackType = void (*)(Connection &conn);

class CommandCenter
{
public:
    CommandCenter();

    void handle(std::string cmd, Connection &conn);
    void addCommand(std::string s, callbackType f);

private:
    std::map<std::string, callbackType> commands_;

};

#endif