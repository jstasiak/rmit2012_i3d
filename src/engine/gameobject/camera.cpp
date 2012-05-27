#include "precompile.h"

#include "camera.h"
#include "../../app/ship.h"

#include "../application.h"
#include "gameobjectset.h"
#include "transform.h"
#include "../scene.h"

REGISTER(Camera);

Camera::Camera()
	:
	BaseGameObject(),
	normalizedRect(),
	backgroundColor(),
	depth(),
	trackedObject(),
	ownerObject(),
	forward()
{

}


void Camera::update(std::shared_ptr< FrameEventArgs > args) {
	BaseGameObject::update(args);

	if(!this->ownerObject.expired()) {
		auto oo = this->ownerObject.lock();
		auto ootransform = oo->getComponents()->getSingleByClass<Transform>();
		auto transform = this->getComponents()->getSingleByClass<Transform>();
		transform->setPosition(ootransform->getPosition());
	}
}



Rectf Camera::getNormalizedRect() {
	return this->normalizedRect;
}

void Camera::setNormalizedRect(Rectf value) {
	this->normalizedRect = value;
}


Recti Camera::getRect() {
	auto app = this->getApplication();

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

	gluPerspective(fovX * aspectRatio, aspectRatio, 0.01f, 300.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	auto& bg = this->backgroundColor;

	glClearColor(bg.x, bg.y, bg.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(!this->trackedObject.expired() && !this->ownerObject.expired()) {
		auto to = this->trackedObject.lock();
		auto oo = this->ownerObject.lock();

		auto ownerPosition = oo->getComponents()->getSingleByClass<Transform>()->getPosition();
		auto trackedPosition = to->getComponents()->getSingleByClass<Transform>()->getPosition();

		auto ownerToTracked = trackedPosition - ownerPosition;
		ownerToTracked = glm::normalize(ownerToTracked);

		auto cameraPosition = ownerPosition - 50.0f * ownerToTracked + glm::vec3(0, 50, 0);

		this->forward = trackedPosition - cameraPosition;
		this->forward = glm::normalize(this->forward);
	
		gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
			trackedPosition.x, trackedPosition.y, trackedPosition.z,
			0, 1, 0);
	}
}


std::weak_ptr<BaseGameObject> Camera::getTrackedObject() {
	return this->trackedObject;
}

void Camera::setTrackedObject(std::shared_ptr<BaseGameObject> value) {
	this->trackedObject = value;
}


std::weak_ptr<BaseGameObject> Camera::getOwnerObject() {
	return this->ownerObject;
}

void Camera::setOwnerObject(std::shared_ptr<BaseGameObject> value) {
	this->ownerObject = value;
}

glm::vec3 Camera::getForward() const {
	return this->forward;
}
