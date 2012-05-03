#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <map>

typedef std::vector < std::string > command_parameters;
typedef std::function<void(command_parameters)> command_function;

class CommandSystem {
private: std::map<std::string, command_function> commands;

public: void registerCommand(std::string name, command_function command);
public: void unregisterCommand(std::string name);
public: void safeExecuteCommand(std::string name);
public: bool commandExists(std::string name) const;
public: command_function getCommand(std::string name) const;
};



#endif // COMMAND_H