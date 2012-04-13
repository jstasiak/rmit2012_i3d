#ifndef MYAPP_H
#define MYAPP_H

#include "../engine/application.h"

class Water;
class Ship;

class MyApp : public Application {
private: bool wireframe;
private: boost::scoped_ptr<Water> water;
private: boost::scoped_ptr<Ship> ship;

public: MyApp();
public: virtual ~MyApp();

protected: virtual void initialize();

private: void initializeGraphics();
private: void initializeCommands();
private: void initializeKeyBindings();

protected: virtual void update(FrameEventArgs* args);
protected: virtual void draw(FrameEventArgs* args);

protected: void applyCameraTransform() const;

public: void toggleWireframe();
};

#endif // MYAPP_H