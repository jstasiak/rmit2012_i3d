#pragma once
class FrameEventArgs;

class Application
{
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

protected:
	virtual void update(FrameEventArgs* args);
	virtual void draw(FrameEventArgs* args);
};

