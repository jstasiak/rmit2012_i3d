#ifndef MYAPP_H
#define MYAPP_H

#include "engine/application.h"

class MyApp : public Application {
protected:
	virtual void update(FrameEventArgs* args);
	virtual void draw(FrameEventArgs* args);
};

#endif // MYAPP_H