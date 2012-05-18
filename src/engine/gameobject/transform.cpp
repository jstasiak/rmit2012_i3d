#include "precompile.h"
#include "transform.h"

REGISTER(Transform);

Transform::Transform()
	: position(),
	rotationDegrees()
{
}


glm::vec3 Transform::getPosition() const {
	return this->position;
}

void Transform::setPosition(glm::vec3 value) {
	this->position = value;
}


glm::vec3 Transform::getRotationDegrees() const {
	return this->rotationDegrees;
}

void Transform::setRotationDegrees(glm::vec3 value) {
	this->rotationDegrees = value;
}