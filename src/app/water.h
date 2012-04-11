#include "precompile.h"

#ifndef WATER_H
#define WATER_H

#include "wave.h"

class Water {
private:
	glm::vec2 size;
	glm::ivec2 segments;

	boost::scoped_ptr<Wave> waveX;
	boost::scoped_ptr<Wave> waveZ;

public:
	Water();
	virtual ~Water();
	void draw();

private:
	float heightAtPositionAndTime(const glm::vec3* position, float time) const;
};

#endif // WATER_H