#ifndef WATER_H
#define WATER_H

#include <glm/glm.hpp>

class Water {
private:
	glm::vec2* size;
	glm::ivec2* segments;

public:
	Water();
	virtual ~Water();
	void draw();

};

#endif // WATER_H