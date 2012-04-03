#include "transform.h"

Transform::Transform() : position(new glm::vec3(0, 0, 0)) {

}

Transform::~Transform() {
	assert(this->position);
	delete this->position;
	this->position = 0;
}

const glm::vec3* Transform::getPosition() const {
	return this->position;
}

void Transform::setPosition( glm::vec3* value ) {
	*(this->position) = *(value);
}

void Transform::translateLocal( glm::vec3* value )
{
}
