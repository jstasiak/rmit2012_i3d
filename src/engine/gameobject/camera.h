#ifndef CAMERA_H
#define CAMERA_H

#include <list>
#include <boost/smart_ptr.hpp>

#include "basegameobject.h"
#include "../utils.h"

class BaseGameObject;

class Camera : public BaseGameObject {
	Q_OBJECT

private: Rectf normalizedRect;
private: glm::vec3 backgroundColor;
private: float depth;
private: std::weak_ptr<BaseGameObject> trackedObject;


public: Q_INVOKABLE Camera();
public: Rectf getNormalizedRect() const;
public: void setNormalizedRect(Rectf value);

public: Recti getRect() const;

public: glm::vec3 getBackgroundColor() const;
public: void setBackgroundColor(glm::vec3 value);

public: float getDepth() const;
public: void setDepth(float value);

public: void applyCamera();

public: std::weak_ptr<BaseGameObject> getTrackedObject();
public: void setTrackedObject(std::shared_ptr<BaseGameObject> value);

};

#endif