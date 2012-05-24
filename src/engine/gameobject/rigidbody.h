#include "basecomponent.h"

#include <glm/glm.hpp>

class RigidBody : public BaseComponent {
	Q_OBJECT

public: static const glm::vec3 EARTH_ACCELERATION;

private: float mass;
private: float radius;

private: glm::vec3 velocity;
private: bool gravityEnabled;

public: Q_INVOKABLE RigidBody();

public: virtual void update(std::shared_ptr< FrameEventArgs > args);

public: glm::vec3 getVelocity() const;
		
public: void applyForce(glm::vec3 force, float seconds);

public: bool getGravityEnabled() const;
public: void setGravityEnabled(bool value);
};