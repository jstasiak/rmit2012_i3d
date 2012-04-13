#ifndef MYAPP_H
#define MYAPP_H

#include "../engine/application.h"

class Water;
class Ship;

class MyApp : public Application {
	enum DrawAxes {
		WorldOrigin,
		WaterOrigin,
		ShipOrigin,
		ShipOriginWithRotation,
	};

private: bool wireframe;
private: boost::scoped_ptr<Water> water;
private: boost::scoped_ptr<Ship> ship;
private: DrawAxes axes;

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
public: void toggleAxes();
};

#endif // MYAPP_H