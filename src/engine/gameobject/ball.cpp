#include "precompile.h"

#include "ball.h"

REGISTER(Ball);

Ball::Ball()
	: radius(1.0f),
	quadric(0)
{}

Ball::~Ball() {
	if(this->quadric) {
		gluDeleteQuadric(this->quadric);
		this->quadric = 0;
	}
}


float Ball::getRadius() const {
	return this->radius;
}

void Ball::setRadius(float value) {
	assert(value > 0);
	this->radius = value;
}


void Ball::start() {
	this->quadric = gluNewQuadric();
}

void Ball::draw(std::shared_ptr< FrameEventArgs > args) {
	gluSphere(this->quadric, this->radius, 30, 30);
}