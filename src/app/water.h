#ifndef WATER_H
#define WATER_H

#include "wave.h"

#include "../engine/gameobject/basegameobject.h"

struct Vertex {
	float position[3];
	float normal[3];
};

class Texture;

class Water : public BaseGameObject {
	Q_OBJECT

private: bool axesVisible;
private: bool normalsVisible;
private: glm::vec2 size;

private: std::shared_ptr< Texture > waterTexture;

public: static const int MIN_SEGMENTS = 1;
public: static const int MAX_SEGMENTS = 128;

private: int segments;

private: boost::scoped_array<Vertex> vertices;

private: boost::scoped_ptr<Wave> waveX;
private: boost::scoped_ptr<Wave> waveZ;

private: float time;

public: Q_INVOKABLE Water();

public: virtual ~Water() {}

public: void resetData();
private: int verticesCount();
private: int indicesCount();
private: int vertexIndex(int x, int z);
public: void recalculate();

// BaseGameObject overrides
public: virtual void start();
public: virtual void update(std::shared_ptr<FrameEventArgs> args);
public: virtual void draw(std::shared_ptr<FrameEventArgs> args);


public: float heightAtPositionAndTime(const glm::vec3* position, float time) const;
public: glm::vec3 normalAtPositionAndTime(const glm::vec3* position, float time) const;

public: void drawNormals();

public: void toggleNormals();

public: Water* doubleTesselationSafe();
public: Water* halveTesselationSafe();

public: void setAxesVisible(bool value);

};

#endif