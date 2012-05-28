#include "precompile.h"

#include "rigidbody.h"
#include "basegameobject.h"
#include "transform.h"

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

void RigidBody::setVelocity(glm::vec3 value) {
	this->velocity = value;
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

float RigidBody::getRadius() const {
	return this->radius;
}

void RigidBody::setRadius(float value) {
	this->radius = value;
}

bool RigidBody::collidesWith(std::shared_ptr< RigidBody> otherBody) {
	assert(this);
	auto thisTransform = this->getGameObject()->getComponents()->getSingleByClass<Transform>();
	auto otherTransform = otherBody->getGameObject()->getComponents()->getSingleByClass<Transform>();

	auto posA = thisTransform->getPosition();
	auto posB = otherTransform->getPosition();
	auto v = posB - posA;
	float distance = glm::length(v);

	return distance < (this->radius + otherBody->getRadius());
}