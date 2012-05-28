#ifndef PLANE_H
#define PLANE_H

#include "../engine/gameobject/basegameobject.h"

struct Vertex {
	float position[3];
	float normal[3];
	float texcoord[2];
};

class Plane : public BaseGameObject {

public: static const int MIN_SEGMENTS = 1;
public: static const int MAX_SEGMENTS = 128;


protected: int segments;

private: float time;


protected: boost::scoped_array<Vertex> vertices;
protected: bool axesVisible;
protected: bool normalsVisible;
protected: glm::vec2 size;


public: Plane();

public: virtual ~Plane();

protected: void resetData();
protected: int verticesCount();
protected: int vertexIndex(int x, int z);
public: void recalculateForPositionAndTime(glm::vec3 position, float time);

protected: void drawNormals();

public: void toggleNormals();

public: void doubleTesselationSafe();
public: void halveTesselationSafe();

public: void setAxesVisible(bool value);


public: virtual float heightAtPositionAndTime(const glm::vec3* position, float time) const = 0;
public: virtual glm::vec3 normalAtPositionAndTime(const glm::vec3* position, float time) const = 0;
public: virtual glm::vec2 texcoordAtPositionAndTime(const glm::vec3& position, const float& time) const = 0;

public: virtual void start();
public: virtual void update(std::shared_ptr<FrameEventArgs> args);
public: void draw(std::shared_ptr<FrameEventArgs> args);

		
public: virtual void setupMaterial();



};

#endif