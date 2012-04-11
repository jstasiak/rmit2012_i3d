#ifndef MYAPP_H
#define MYAPP_H

#include "../engine/application.h"

class Water;
class Ship;

class MyApp : public Application {
private:
	boost::scoped_ptr<Water> water;
	boost::scoped_ptr<Ship> ship;
public:
	MyApp();
	virtual ~MyApp();
protected:
	virtual void initialize();
	virtual void update(FrameEventArgs* args);
	virtual void draw(FrameEventArgs* args);
};

#endif // MYAPP_H