#ifndef MYAPP_H
#define MYAPP_H

#include "../engine/application.h"

class Water;
class Ship;

class MyApp : public Application {
public: enum DrawAxes {
		WorldOrigin,
		WaterOrigin,
		ShipOrigin,
		ShipOriginWithRotation,
	};

private: bool wireframe;
private: boost::shared_ptr<Water> water;
private: boost::shared_ptr<Ship> ship;
private: DrawAxes axes;

public: MyApp();
public: virtual ~MyApp();

protected: virtual void initialize();

private: void initializeGraphics();
private: void initializeCommands();
private: void initializeKeyBindings();

protected: virtual void update(boost::shared_ptr<FrameEventArgs> args);
protected: virtual void draw(boost::shared_ptr<FrameEventArgs> args);

protected: void enableLights();

protected: void applyCameraTransform() const;

public: void toggleWireframe();
public: void toggleAxes();
};

#endif // MYAPP_H