#include "precompile.h"

#include "camera.h"
#include "../../app/ship.h"

#include "../application.h"

REGISTER(Camera);

Camera::Camera() : BaseGameObject(), rect(), backgroundColor(), depth()
{

}

Rectf Camera::getRect() const {
	return this->rect;
}

void Camera::setRect(Rectf value) {
	this->rect = value;
}

glm::vec3 Camera::getBackgroundColor() const {
	return this->backgroundColor;
}

void Camera::setBackgroundColor(glm::vec3 value) {
	this->backgroundColor = value;
}

float Camera::getDepth() const {
	return this->depth;
}

void Camera::setDepth(float value) {
	this->depth = value;
}

void Camera::applyCamera() {
	auto app = this->getGameObjectSet().lock()->getApplication().lock();
	auto screenSize = app->getScreenSize();

	float x = this->rect.x * screenSize.x;
	float y = this->rect.y * screenSize.y;
	float w = this->rect.width * screenSize.x;
	float h = this->rect.height * screenSize.y;

	glViewport(x, y, w, h);
	glScissor(x, y, w, h);
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.33f, 0.01f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	auto& bg = this->backgroundColor;

	glClearColor(bg.x, bg.y, bg.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto ship = this->gameObjectSet.lock()->getSingleByClass<Ship>();
	auto position = *ship->getPosition();
	gluLookAt(0, 100, 100,
		position.x, 0, position.z,
		0, 1, 0);
}