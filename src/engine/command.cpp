#include "precompile.h"

#include "command.h"

using namespace std;

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

void CommandSystem::safeExecuteCommandLine(string commandLine) {
	try {
		this->executeCommandLine(commandLine);
	}
	catch(const exception& e) {
		printf("[CommandSystem] Cannot execute '%s': '%s'\n", commandLine.c_str(), e.what());
	}
}


void CommandSystem::executeCommandLine(std::string commandLine) {
	boost::algorithm::trim(commandLine);

	if(commandLine != string("") && commandLine.at(0) != '#') {
		printf("/%s\n", commandLine.c_str());

		auto tokens = this->parseCommandline(commandLine);
		this->executeCommandTokens(tokens);
	}
}

vector< string > CommandSystem::parseCommandline(string commandLine) {
	vector< string > tokens;

	boost::char_separator<char> separator(" \t");
	boost::tokenizer<boost::char_separator<char> > tokenizer(commandLine, separator);

	BOOST_FOREACH(string token, tokenizer) {
		tokens.push_back(token);
	}
	
	return tokens;
}

void CommandSystem::executeCommandTokens(vector< string > tokens) {
	auto commandName = tokens.at(0);
	tokens.erase(tokens.begin());

	if(this->commandExists(commandName)) {
		auto function = this->getCommand(commandName);
		function(tokens);
	} else {
		throw exception("Command does not exist");
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