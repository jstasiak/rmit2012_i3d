#ifndef SHIP_H
#define SHIP_H

#include "glm/glm.hpp"
#include "obj.h"

#include "../engine/basegameobject.h"

class Water;

class Ship : public BaseGameObject {
public: enum ShipDrawAxes {
	DontDraw,
	Draw,
	DrawWithRotation
};
private: OBJMesh* mesh;
private: glm::vec3 position;
private: float yaw;
private: Water* water;
private: ShipDrawAxes axes;

private: static const float MIN_VELOCITY;
private: static const float MAX_VELOCITY;
private: static const float ACCELERATION;

private: float currentAcceleration;
private: float velocity;


private: static const float TURNING_SPEED_DEGREES_PER_SECOND;

private: float currentTurningSpeedDegreesPerSecond;



public: Ship(Water* water);
public: virtual ~Ship();

public: void startAcceleration();
public: void startDecceleration();
public: void stopAcceleration();

public: void startTurningLeft();
public: void stopTurningLeft();

public: void startTurningRight();
public: void stopTurningRight();

// BaseGameObject overrides
public: virtual void update(FrameEventArgs* args);
public: virtual void draw(FrameEventArgs* args);

public: glm::vec3* getPosition();
public: Ship* setPosition(const glm::vec3* value);

public: void setAxes(ShipDrawAxes axes);
};

#endif // SHIP_H