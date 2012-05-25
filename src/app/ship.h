#ifndef SHIP_H
#define SHIP_H

#include "../engine/gameobject/basegameobject.h"

#include "obj.h"
#include "glm/glm.hpp"


class Water;

class Ship : public BaseGameObject {
	Q_OBJECT

public: enum ShipDrawAxes {
	DontDraw,
	Draw,
	DrawWithRotation
};

private: OBJMesh* mesh;
private: float yaw;
private: std::shared_ptr<Water> water;
private: ShipDrawAxes axes;
		 
private: static const float MIN_VELOCITY;
private: static const float MAX_VELOCITY;
private: static const float ACCELERATION;

private: float currentAcceleration;
private: float velocity;

private: bool useWaterLevel;


private: static const float TURNING_SPEED_DEGREES_PER_SECOND;

private: float currentTurningSpeedDegreesPerSecond;

public: Q_INVOKABLE Ship();

public: virtual ~Ship();

public: void startAcceleration();
public: void startDecceleration();
public: void stopAcceleration();

public: void startTurningLeft();
public: void stopTurningLeft();

public: void startTurningRight();
public: void stopTurningRight();

public: void fire(std::string side);

// BaseGameObject overrides
public: virtual void start();
public: virtual void fixedUpdate(std::shared_ptr<FrameEventArgs> args);
public: virtual void update(std::shared_ptr<FrameEventArgs> args);
public: virtual void draw(std::shared_ptr<FrameEventArgs> args);

public: void setAxes(ShipDrawAxes axes);

public: bool getUseWaterLevel() const;
public: void setUseWaterLevel(bool value);
};

#endif