#include "precompile.h"

#include "camera.h"
#include "../../app/ship.h"

#include "../application.h"
#include "gameobjectset.h"
#include "transform.h"

REGISTER(Camera);

Camera::Camera()
	: BaseGameObject(), normalizedRect(), backgroundColor(), depth(), trackedObject()
{

}

Rectf Camera::getNormalizedRect() const {
	return this->normalizedRect;
}

void Camera::setNormalizedRect(Rectf value) {
	this->normalizedRect = value;
}

Recti Camera::getRect() const {
	auto app = this->gameObjectSet.lock()->getOwner();

	auto screenSize = app->getScreenSize();
	auto w = screenSize.x;
	auto h = screenSize.y;

	auto& r = this->normalizedRect;
	return Recti(r.x * w, r.y * h, r.width * w, r.height * h);
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
	auto rect = this->getRect();

	float x = rect.x;
	float y = rect.y;
	float w = rect.width;
	float h = rect.height;

	glViewport(x, y, w, h);
	glScissor(x, y, w, h);
	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspectRatio = rect.width * 1.0f / rect.height;
	float fovX = 90.0f;

	gluPerspective(fovX * aspectRatio, aspectRatio, 0.01f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	auto& bg = this->backgroundColor;

	glClearColor(bg.x, bg.y, bg.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!this->trackedObject.expired()) {
		auto to = this->trackedObject.lock();
		auto position = to->getComponents()->getSingleByClass<Transform>()->getPosition();
	
		gluLookAt(0, 100, 100,
			position.x, 0, position.z,
			0, 1, 0);
	}
}

std::weak_ptr<BaseGameObject> Camera::getTrackedObject() {
	return this->trackedObject;
}

void Camera::setTrackedObject(std::shared_ptr<BaseGameObject> value) {
	this->trackedObject = value;
}