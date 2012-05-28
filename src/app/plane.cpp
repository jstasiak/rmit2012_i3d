#include "precompile.h"

#include "plane.h"

#include "../engine/gameobject/camera.h"
#include "../engine/gameobject/gameobjectset.h"
#include "../engine/gameobject/transform.h"
#include "../engine/scene.h"

Plane::Plane()
	: vertices(0),
	size(600, 600),
	segments(64),
	normalsVisible(false),
	axesVisible(false)
{
	this->resetData();
}

void Plane::resetData() {
	this->vertices.reset(new Vertex[this->verticesCount()]);
}

Plane::~Plane() {
}


// Function to map x and z coordinate of plane point
// to one-dimensional array coordinate
int Plane::vertexIndex(int x, int z) {
	return x * (this->segments + 1) + z;
}


void Plane::recalculateForPositionAndTime(glm::vec3 position, float time) {
	// We center plane at given position so there's always
	// water around us

	auto center = position;
	center.y = 0;

	glm::vec2 segmentSize(this->size.x / this->segments,
		this->size.y / this->segments);

	// Minimal coordinates water corner
	auto start = center - glm::vec3(this->size.x * 0.5f, 0, -this->size.y * 0.5f);

	// Variables to provide access to vertices array
	Vertex* allVertices = this->vertices.get();
	Vertex* p = 0;

	// Variables to store temporary vertex data
	glm::vec3 vertexPosition;
	glm::vec3 normal;
	glm::vec2 texcoord;

	for(int xpoint = 0; xpoint <= this->segments; ++xpoint) {
		for(int zpoint = 0; zpoint <= this->segments; ++zpoint) {
			// Get vertex pointer within the array
			p = allVertices + this->vertexIndex(xpoint, zpoint);

			// Calculate vertex position and normal
			vertexPosition = start + glm::vec3(xpoint * segmentSize.x, 0, -zpoint * segmentSize.y);
			vertexPosition.y = this->heightAtPositionAndTime(&vertexPosition, time);
			normal = this->normalAtPositionAndTime(&vertexPosition, time);
			texcoord = this->texcoordAtPositionAndTime(vertexPosition, time);

			// Copy to buffer
			memcpy(p->position, glm::value_ptr(vertexPosition), 3 * sizeof(float));
			memcpy(p->normal, glm::value_ptr(normal), 3 * sizeof(float));
			memcpy(p->texcoord, glm::value_ptr(texcoord), 2 * sizeof(float));
		}
	}
}

int Plane::verticesCount() {
	return (this->segments + 1) * (this->segments + 1);
}


void Plane::start() {
}

void Plane::update(std::shared_ptr< FrameEventArgs > args) {
}


void Plane::draw(std::shared_ptr< FrameEventArgs > args) {
	auto scene = this->getGameObjectSet().lock()->getOwner();
	auto camera = scene->getActiveCamera();
	auto cameraTransform = camera->getComponents()->getSingleByClass<Transform>();

	this->recalculateForPositionAndTime(cameraTransform->getPosition(), args->getTotalSeconds());

	if(this->axesVisible) {
		drawAxes(50);
	}

	Vertex* base = this->vertices.get();
	Vertex* v1 = 0;
	Vertex* v2 = 0;
	Vertex* v3 = 0;
	Vertex* v4 = 0;

		
	this->setupMaterial();

	glBegin(GL_TRIANGLES);

	for(int xsegment = 0; xsegment < this->segments; ++xsegment) {
		for(int zsegment = 0; zsegment < this->segments; ++zsegment) {
			v1 = base + this->vertexIndex(xsegment, zsegment);
			v2 = base + this->vertexIndex(xsegment + 1, zsegment);
			v3 = base + this->vertexIndex(xsegment + 1, zsegment + 1);
			v4 = base + this->vertexIndex(xsegment, zsegment + 1);

			// These normals are here only to show lighting effect, they're not proper normals

			glNormal3fv(v1->normal);
			glTexCoord2fv(v1->texcoord);
			glVertex3fv(v1->position);

			glNormal3fv(v2->normal);
			glTexCoord2fv(v2->texcoord);
			glVertex3fv(v2->position);

			glNormal3fv(v4->normal);
			glTexCoord2fv(v4->texcoord);
			glVertex3fv(v4->position);


			glNormal3fv(v2->normal);
			glTexCoord2fv(v2->texcoord);
			glVertex3fv(v2->position);

			glNormal3fv(v3->normal);
			glTexCoord2fv(v3->texcoord);
			glVertex3fv(v3->position);

			glNormal3fv(v4->normal);
			glTexCoord2fv(v4->texcoord);
			glVertex3fv(v4->position);
		}
	}
		
	glEnd();
	glDisable(GL_TEXTURE_2D);

	if(this->normalsVisible) {
		this->drawNormals();
	}
}

void Plane::setupMaterial() {
	glColor3f(1.0f, 1.0f, 0.0f);
}

void Plane::drawNormals() {
	Vertex* base = this->vertices.get();
	Vertex* v = 0;

	glColor3f(1.0f, 1.0f, 0.0f);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 1, 1);

		glVertex3f(30, 0, 30);
		glVertex3f(30, 10, 30);

		for(int xpoint = 0; xpoint <= this->segments; ++xpoint) {
			for(int zpoint = 0; zpoint <= this->segments; ++zpoint) {
				v = base + this->vertexIndex(xpoint, zpoint);
				glVertex3fv(v->position);
				glVertex3f(v->position[0] + v->normal[0] * 10,
					v->position[1] + v->normal[1] * 10,
					v->position[2] + v->normal[2] * 10);
			}
		}
	glEnd();

	glEnable(GL_LIGHTING);
}

void Plane::toggleNormals() {
	this->normalsVisible = !this->normalsVisible;
	printf("[Water] Normals visible: %d\n", this->normalsVisible);
}

void Plane::doubleTesselationSafe() {
	if(this->segments * 2 <= Plane::MAX_SEGMENTS) {
		this->segments *= 2;
	}

	this->resetData();
}

void Plane::halveTesselationSafe() {
	if(this->segments / 2 >= Plane::MIN_SEGMENTS) {
		this->segments /= 2;
	}

	this->resetData();
}

void Plane::setAxesVisible(bool value) {
	this->axesVisible = value;
}