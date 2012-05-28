#include "basecomponent.h"

#include <glm/glm.hpp>

class RigidBody : public BaseComponent {
public: virtual const char* className() const {
		return "RigidBody";
	}

public: static const glm::vec3 EARTH_ACCELERATION;

private: float mass;
private: float radius;

private: glm::vec3 velocity;
private: bool gravityEnabled;

public: RigidBody();

public: virtual void update(std::shared_ptr< FrameEventArgs > args);

public: glm::vec3 getVelocity() const;

public: void setVelocity(glm::vec3 value);

public: void applyForce(glm::vec3 force, float seconds);

public: bool getGravityEnabled() const;
public: void setGravityEnabled(bool value);

public: float getRadius() const;
public: void setRadius(float value);

public: bool collidesWith(std::shared_ptr< RigidBody> otherBody);
};