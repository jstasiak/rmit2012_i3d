#ifndef WATER_H
#define WATER_H

#include "wave.h"

#include "plane.h"

class Texture;
class PerlinNoise;

class Water : public Plane {
	Q_OBJECT

private: std::shared_ptr< Texture > waterTexture;
private: std::shared_ptr< PerlinNoise > noise;


private: boost::scoped_ptr<Wave> waveX;
private: boost::scoped_ptr<Wave> waveZ;


public: Q_INVOKABLE Water();


public: virtual void start();

public: virtual void setupMaterial();

public: virtual float heightAtPositionAndTime(const glm::vec3* position, float time) const;
public: virtual glm::vec3 normalAtPositionAndTime(const glm::vec3* position, float time) const;
public: virtual glm::vec2 texcoordAtPositionAndTime(const glm::vec3& position, const float& time) const;

};

#endif