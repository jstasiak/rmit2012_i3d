#ifndef SHIP_H
#define SHIP_H

#include "glm/glm.hpp"

#include "obj.h"

class Ship {
private:
	OBJMesh* mesh;
private:
	glm::vec3 position;
public:
	Ship();
public:
	virtual ~Ship();
public:
	void draw() const;
public:
	glm::vec3* getPosition();
public:
	Ship* setPosition(const glm::vec3* value);

};

#endif // SHIP_H