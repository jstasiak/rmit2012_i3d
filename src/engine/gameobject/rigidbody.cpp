#include "precompile.h"

#include "rigidbody.h"
#include "basegameobject.h"
#include "transform.h"

REGISTER(RigidBody);

const glm::vec3 RigidBody::EARTH_ACCELERATION(0, -9.81, 0);

RigidBody::RigidBody()
	: BaseComponent(),
	mass(1),
	radius(1),
	velocity(0, 0, 0),
	gravityEnabled(true)
{
}

void RigidBody::update(std::shared_ptr< FrameEventArgs > args) {
	auto dt = args->getSeconds();

	if(this->gravityEnabled) {
		this->applyForce(RigidBody::EARTH_ACCELERATION * this->mass, dt);
	}

	auto go = this->getGameObject();
	auto transform = go->getComponents()->getSingleByClass<Transform>();

	auto dposition = this->velocity * dt;
	transform->setPosition(transform->getPosition() + dposition);
}

glm::vec3 RigidBody::getVelocity() const {
	return this->velocity;
}

void RigidBody::applyForce(glm::vec3 force, float seconds) {
	auto acceleration = force * (1.0f / this->mass);
	auto dvelocity = acceleration * seconds;
	this->velocity += dvelocity;
}


bool RigidBody::getGravityEnabled() const {
	return this->gravityEnabled;
}

void RigidBody::setGravityEnabled(bool value) {
	this->gravityEnabled = value;
}