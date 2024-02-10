#include "command.h"

CommandCenter::CommandCenter()
{

}

void CommandCenter::addCommand(std::string cmd, callbackType func)
{
    commands_[cmd] = func;
}

void CommandCenter::handle(std::string cmd, Connection &conn)
{
    auto search = commands_.find(cmd);

    if (search != commands_.end())
        commands_[cmd](conn);
}