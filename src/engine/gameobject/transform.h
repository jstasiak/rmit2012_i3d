#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "basecomponent.h"

class Transform : public BaseComponent {
	Q_OBJECT

private: glm::mat4x4 matrix;

public: Q_INVOKABLE Transform();

public: glm::vec3 getPosition() const;
public: void setPosition(glm::vec3 value);


public: glm::mat4x4 getMatrix();

public: glm::vec3 getForward();
public: glm::vec3 getUp();
public: glm::vec3 getRight();

public: void translateGlobal(glm::vec3 value);
public: void translateLocal(glm::vec3 value);

public: void rotateAroundGlobal(glm::vec3 axis, float degrees);
public: void rotateAroundLocal(glm::vec3 axis, float degrees);

};


#endif