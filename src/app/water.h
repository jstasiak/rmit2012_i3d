#include "precompile.h"

#ifndef WATER_H
#define WATER_H

#include "wave.h"

class Water {
private:
	glm::vec2 size;
	glm::ivec2 segments;

	boost::scoped_ptr<Wave> waveX;
	boost::scoped_ptr<Wave> waveY;

public:
	Water();
	virtual ~Water();
	void draw();
};

#endif // WATER_H