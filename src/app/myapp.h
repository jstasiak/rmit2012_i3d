#ifndef MYAPP_H
#define MYAPP_H

#include "../engine/application.h"

class Water;
class Ship;

class MyApp : public Application {


public: MyApp();
public: virtual ~MyApp();

protected: virtual void initialize();
protected: virtual void beforeDraw(std::shared_ptr<FrameEventArgs> args);

protected: void initializeCommands();

protected: void enableLights();

protected: void applyCameraTransform() const;
};

#endif // MYAPP_H