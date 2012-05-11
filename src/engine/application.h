#ifndef APPLICATION_H
#define APPLICATION_H

#include <boost/smart_ptr.hpp>
#include <SDL.h>

#include "frameeventargs.h"

class GameObjectSet;
class CommandSystem;

class Application : public std::enable_shared_from_this<Application>
{
protected:
	std::shared_ptr<CommandSystem> commandSystem;
	std::map<SDLKey, std::string> bindings;
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


private: void onKeyDown(const SDL_KeyboardEvent* event);
private: void onKeyUp(const SDL_KeyboardEvent* event);

protected: virtual void initialize() {}
protected: virtual void beforeDraw(std::shared_ptr<FrameEventArgs> args) {}

protected: void quit();

public: std::shared_ptr<CommandSystem> getCommandSystem();
public: std::shared_ptr<Application> getSharedPointer();

public: std::string getDataDirectory() const;

};

#endif // APPLICATION_H