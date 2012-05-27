#ifndef APPLICATION_H
#define APPLICATION_H

#include <boost/smart_ptr.hpp>
#include <SDL.h>

#include "frameeventargs.h"

class Camera;
class CommandSystem;
class GameObjectSet;
class Scene;

class Application : public std::enable_shared_from_this<Application>
{
private: SDL_Surface* surface;

protected:
	std::shared_ptr<CommandSystem> commandSystem;
	std::map<std::string, std::string> bindings;
	std::shared_ptr<Scene> scene;
	std::string gameDir;

private:
	int desiredFixedUpdateFps;
	int desiredDrawFps;

	float updateEverySeconds;
	float drawEverySeconds;

private: float currentDrawFps;

public: void setDesiredFixedUpdateFps(int value);
public: void setDesiredDrawFps(int value);

public: Application();
public: ~Application();

public: int runWithCommandLineParameters(int argc, char** argv);
private: int run();

private: void initialize();
private: void initializeCommands();
private: void initializeGraphics();

private: void executeConfigFile(std::string configFile);

private: void update(std::shared_ptr<FrameEventArgs> args);
private: void draw(std::shared_ptr<FrameEventArgs> args);

private: void onKeyDown(const SDL_KeyboardEvent* event);
private: void onKeyUp(const SDL_KeyboardEvent* event);

private: void quit();

public: std::shared_ptr<CommandSystem> getCommandSystem();
public: std::shared_ptr<Application> getSharedPointer();

public: std::string getDataDirectory() const;

public: glm::ivec2 getScreenSize() const;

public: float getCurrentDrawFps() const;

};

#endif