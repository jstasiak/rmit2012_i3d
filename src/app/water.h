#ifndef WATER_H
#define WATER_H

#include "wave.h"

class Water {
private: bool normalsVisible;
private: glm::vec2 size;

private: int segments;

public: static const int MIN_SEGMENTS = 1;
public: static const int MAX_SEGMENTS = 128;

private: boost::scoped_ptr<Wave> waveX;
private: boost::scoped_ptr<Wave> waveZ;

public: Water();

public: void draw();

public: float heightAtPositionAndTime(const glm::vec3* position, float time) const;
public: glm::vec3 normalAtPositionAndTime(const glm::vec3* position, float time) const;

public: bool getNormalsVisible() const;
public: Water* setNormalsVisible(bool value);

public: Water* doubleTesselationSafe();
public: Water* halveTesselationSafe();
};

#endif // WATER_H