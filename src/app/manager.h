#ifndef MYAPP_H
#define MYAPP_H

#include "../engine/gameobject/basecomponent.h"

class Water;
class Ship;
class Font;

class Manager : public BaseComponent {
	Q_OBJECT

private: std::shared_ptr< Font> font;

public: enum DrawAxes {
		WorldOrigin,
		WaterOrigin,
		ShipOrigin,
		ShipOriginWithRotation,
	};

private: bool wireframe;
private: DrawAxes axes;

public: Q_INVOKABLE Manager();

public: virtual void start();
public: virtual void update(std::shared_ptr<FrameEventArgs> args);
public: virtual void onGui();


public: void toggleWireframe();
public: void toggleAxes();
};

#endif