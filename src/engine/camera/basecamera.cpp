#include "basecamera.h"

#include <cassert>

float BaseCamera::getNearClipDistance() const {
	return this->near;
}

void BaseCamera::setNearClipDistance( float value ) {
	assert(value > 0);
	this->near = value;
}

float BaseCamera::getFarClipDistance() const {
	return this->far;
}

void BaseCamera::setFarClipDistance( float value ) {
	assert(value > 0);
	this->far = value;
}
