#ifndef MYAPP_H
#define MYAPP_H

#include "../engine/application.h"

class Water;

class MyApp : public Application {
private:
	Water* water;

public:
	MyApp();
protected:
	virtual void update(FrameEventArgs* args);
	virtual void draw(FrameEventArgs* args);
};

#endif // MYAPP_H