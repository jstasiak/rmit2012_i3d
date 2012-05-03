#include "precompile.h"
#include "command.h"

void CommandSystem::registerCommand(std::string name, command_function command) {
	auto iterator = this->commands.find(name);
	assert(iterator == this->commands.end());
	this->commands[name] = command;
}

void CommandSystem::unregisterCommand(std::string name) {
	auto iterator = this->commands.find(name);
	assert(iterator != this->commands.end());
	this->commands.erase(iterator);
}

void CommandSystem::safeExecuteCommand(std::string name) {
	if(this->commandExists(name)) {
		auto command = this->getCommand(name);
		command(command_parameters());
	} else {
		printf("[CommandSystem] Command does not exist: %s\n", name.c_str());
	}
}

bool CommandSystem::commandExists(std::string name) const {
	auto iterator = this->commands.find(name);
	return iterator != this->commands.end();
}

command_function CommandSystem::getCommand(std::string name) const {
	auto iterator = this->commands.find(name);
	assert(iterator != this->commands.end());

	return (*iterator).second;
}