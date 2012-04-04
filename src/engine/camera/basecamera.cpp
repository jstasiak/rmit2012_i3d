#include "precompile.h"
#include "basecamera.h"

#include <cassert>

float BaseCamera::getNearClipDistance() const {
	return this->n;
}

void BaseCamera::setNearClipDistance( float value ) {
	assert(value > 0);
	this->n = value;
}

float BaseCamera::getFarClipDistance() const {
	return this->f;
}

void BaseCamera::setFarClipDistance( float value ) {
	assert(value > 0);
	this->f = value;
}
