#ifndef APPLICATION_H
#define APPLICATION_H

#include <boost/smart_ptr.hpp>
#include <SDL.h>

#include "frameeventargs.h"

class Camera;
class CommandSystem;
class GameObjectSet;


class Application : public std::enable_shared_from_this<Application>
{
private: SDL_Surface* surface;

protected:
	std::shared_ptr<CommandSystem> commandSystem;
	std::map<std::string, std::string> bindings;
	std::shared_ptr<GameObjectSet> gameObjectSet;
	std::string gameDir;

private:
	int updateFps;
	int drawFps;

	float updateEverySeconds;
	float drawEverySeconds;

public: void setUpdateFps(int value);
public: void setDrawFps(int value);

public:
	Application();
	virtual ~Application();

	virtual void applyCommandlineParameters(int argc, char** argv);
	int run();

private:
	void doInitialize();
	void doUpdate(std::shared_ptr<FrameEventArgs> args);
	void doDraw(std::shared_ptr<FrameEventArgs> args);

private: std::list< std::shared_ptr< Camera > > getSortedCameras() const;
private: void drawGameObjects(std::shared_ptr<FrameEventArgs> args);

protected: void executeConfigFile();

private: void onKeyDown(const SDL_KeyboardEvent* event);
private: void onKeyUp(const SDL_KeyboardEvent* event);

protected: void quit();

public: std::shared_ptr<CommandSystem> getCommandSystem();
public: std::shared_ptr<Application> getSharedPointer();

public: std::string getDataDirectory() const;

public: glm::ivec2 getScreenSize() const;

private: void enableLights();

};

#endif