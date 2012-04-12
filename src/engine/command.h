#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <map>

typedef std::function<void()> command_function;

class CommandSystem {
private: std::map<std::string, command_function> commands;

public: CommandSystem* registerCommand(std::string name, command_function command);
public: CommandSystem* unregisterCommand(std::string name);
public: CommandSystem* safeExecuteCommand(std::string name);
public: bool commandExists(std::string name) const;
public: command_function getCommand(std::string name) const;
};



#endif // COMMAND_H