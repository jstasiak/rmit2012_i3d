#ifndef APPLICATION_H
#define APPLICATION_H

#include <boost/smart_ptr.hpp>
#include <SDL.h>

#include "command.h"
#include "frameeventargs.h"

class Application
{
protected:
	boost::scoped_ptr<CommandSystem> commandSystem;
	std::map<SDLKey, std::string> bindings;

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
	void doUpdate(boost::shared_ptr<FrameEventArgs> args);
	void doDraw(boost::shared_ptr<FrameEventArgs> args);

protected:
	virtual void initialize();
	virtual void update(boost::shared_ptr<FrameEventArgs> args);
	virtual void draw(boost::shared_ptr<FrameEventArgs> args);
	virtual void onKeyDown(const SDL_KeyboardEvent* event);
	virtual void onKeyUp(const SDL_KeyboardEvent* event);

protected:
	void quit();
};

#endif // APPLICATION_H