#ifndef APPLICATION_H
#define APPLICATION_H

#include <boost/scoped_ptr.hpp>

#include "command.h"
#include "frameeventargs.h"

class Application
{
protected:
	boost::scoped_ptr<CommandSystem> commandSystem;
private:
	int updateFps;
	int drawFps;

	float updateEverySeconds;
	float drawEverySeconds;

	void setUpdateFps(int value);
	void setDrawFps(int value);

public:
	Application();
	virtual ~Application();

	virtual void applyCommandlineParameters(int argc, char** argv);
	int run();

private:
	void doInitialize();
	void doUpdate(FrameEventArgs* args);
	void doDraw(FrameEventArgs* args);

protected:
	virtual void initialize();
	virtual void update(FrameEventArgs* args);
	virtual void draw(FrameEventArgs* args);

protected:
	void quit();
};

#endif // APPLICATION_H