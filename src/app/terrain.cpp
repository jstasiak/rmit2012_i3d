#include "precompile.h"

#include "terrain.h"

#include "../engine/perlinnoise.h"

Terrain::Terrain()
	: noise(new PerlinNoise())
{
}

void Terrain::setupMaterial() {
	glColor3f(1.0f, 1.0f, 0.0f);
}


float Terrain::heightAtPositionAndTime(const glm::vec3* position, float time) const {
	return -5.0f + 30 * this->noise->getValue(position->x / 100.0f, position->z / 100.0f);
}

glm::vec3 Terrain::normalAtPositionAndTime(const glm::vec3* position, float time) const {
	return glm::vec3(0, 1, 0);
}

glm::vec2 Terrain::texcoordAtPositionAndTime(const glm::vec3& position, const float& time) const {
	return glm::vec2(0, 0);
}
