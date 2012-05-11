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
private: DrawAxes axes;

public: MyApp();
public: virtual ~MyApp();

protected: virtual void initialize();
protected: virtual void beforeDraw(std::shared_ptr<FrameEventArgs> args);

protected: void initializeCommands();

protected: void enableLights();

protected: void applyCameraTransform() const;

public: void toggleWireframe();
public: void toggleAxes();
};

#endif // MYAPP_H