#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

class Transform {
private:
	glm::vec3* position;

public:
	Transform();
	virtual ~Transform();

	const glm::vec3* getPosition() const;
	void setPosition(glm::vec3* value);

	void translateLocal(glm::vec3* value);
	void translateWorld(glm::vec3* value);
};

#endif // TRANSFORM_H