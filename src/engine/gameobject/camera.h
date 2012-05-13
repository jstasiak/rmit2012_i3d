#ifndef CAMERA_H
#define CAMERA_H

#include <list>
#include <boost/smart_ptr.hpp>

#include "basegameobject.h"
#include "../utils.h"

class BaseGameObject;

class Camera : public BaseGameObject {
	Q_OBJECT
private: Rectf rect;
private: glm::vec3 backgroundColor;
private: float depth;

public: Q_INVOKABLE Camera();
public: Rectf getRect() const;
public: void setRect(Rectf value);

public: glm::vec3 getBackgroundColor() const;
public: void setBackgroundColor(glm::vec3 value);

public: float getDepth() const;
public: void setDepth(float value);

public: void applyCamera();

};

#endif