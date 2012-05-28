#ifndef CAMERA_H
#define CAMERA_H

#include <list>
#include <boost/smart_ptr.hpp>

#include "basegameobject.h"
#include "../utils.h"

class BaseGameObject;

class Camera : public BaseGameObject {
public: virtual const char* className() const {
		return "Camera";
	}

private: Rectf normalizedRect;
private: glm::vec3 backgroundColor;
private: float depth;
private: std::weak_ptr<BaseGameObject> trackedObject;
private: std::weak_ptr<BaseGameObject> ownerObject;

private: glm::vec3 forward;

public: Camera();

public: virtual void update(std::shared_ptr< FrameEventArgs > args);


public: Rectf getNormalizedRect();
public: void setNormalizedRect(Rectf value);

public: Recti getRect();

public: glm::vec3 getBackgroundColor() const;
public: void setBackgroundColor(glm::vec3 value);

public: float getDepth() const;
public: void setDepth(float value);

public: void applyCamera();

public: std::weak_ptr<BaseGameObject> getTrackedObject();
public: void setTrackedObject(std::shared_ptr<BaseGameObject> value);

public: std::weak_ptr< BaseGameObject > getOwnerObject();
public: void setOwnerObject(std::shared_ptr< BaseGameObject > value);

public: glm::vec3 getForward() const;
};

#endif