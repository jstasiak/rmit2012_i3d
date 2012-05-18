#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "basecomponent.h"

class Transform : public BaseComponent {
	Q_OBJECT

private: glm::vec3 position;
private: glm::vec3 rotationDegrees;

public: Q_INVOKABLE Transform();

public: glm::vec3 getPosition() const;
public: void setPosition(glm::vec3 value);

public: glm::vec3 getRotationDegrees() const;
public: void setRotationDegrees(glm::vec3 value);

};


#endif