#ifndef TERRAIN_H
#define TERRAIN_H

#include "plane.h"

class PerlinNoise;

class Terrain : public Plane {
public: virtual const char* className() const {
		return "Terrain";
	}

private: std::shared_ptr< PerlinNoise > noise;

public: Terrain();


public: virtual void setupMaterial();

public: virtual float heightAtPositionAndTime(const glm::vec3* position, float time) const;
public: virtual glm::vec3 normalAtPositionAndTime(const glm::vec3* position, float time) const;
public: virtual glm::vec2 texcoordAtPositionAndTime(const glm::vec3& position, const float& time) const;

};

#endif