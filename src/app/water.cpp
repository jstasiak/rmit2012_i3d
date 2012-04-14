#include "precompile.h"
#include "water.h"

#include "../engine/utils.h"

Water::Water()
	: size(300, 300),
	segments(32),
	waveX((new Wave())->setAmplitude(4)->setLength(73)->setFrequency(0.7f)),
	waveZ((new Wave())->setAmplitude(4)->setLength(44)->setFrequency(0.5f)),
	time(0),
	vertices(0),
	normalsVisible(false)
{
	this->resetData();

}

void Water::resetData() {
	this->vertices.reset(new Vertex[this->verticesCount()]);
	this->recalculate();
}

int Water::verticesCount() {
	return (this->segments + 1) * (this->segments + 1);
}

int Water::vertexIndex(int x, int z) {
	return x * (this->segments + 1) + z;
}

void Water::recalculate() {
	float time = this->time;
	auto center = glm::vec3(0, 0, 0);

	glm::vec2 segmentSize(this->size.x / this->segments,
		this->size.y / this->segments);

	auto start = center - glm::vec3(this->size.x * 0.5f, 0, -this->size.y * 0.5f);

	Vertex* allVertices = this->vertices.get();
	Vertex* p = 0;

	glm::vec3 position;
	glm::vec3 normal;

	for(int xpoint = 0; xpoint <= this->segments; ++xpoint) {
		for(int zpoint = 0; zpoint <= this->segments; ++zpoint) {
			p = allVertices + this->vertexIndex(xpoint, zpoint);
			position = start + glm::vec3(xpoint * segmentSize.x, 0, -zpoint * segmentSize.y);
			position.y = this->heightAtPositionAndTime(&position, time);
			normal = this->normalAtPositionAndTime(&position, time);

			memcpy(p->position, glm::value_ptr(position), 3 * sizeof(float));
			memcpy(p->normal, glm::value_ptr(normal), 3 * sizeof(float));
		}
	}
}

void Water::update(FrameEventArgs* args) {
	this->time = args->getTotalSeconds();
}



void Water::draw(FrameEventArgs* args) {
	this->recalculate();

	glColor3f(0.0f, 0.2f, 1.0f);

	Vertex* base = this->vertices.get();
	Vertex* v1 = 0;
	Vertex* v2 = 0;
	Vertex* v3 = 0;
	Vertex* v4 = 0;

	glBegin(GL_TRIANGLES);

	for(int xsegment = 0; xsegment < this->segments; ++xsegment) {
		for(int zsegment = 0; zsegment < this->segments; ++zsegment) {
			v1 = base + this->vertexIndex(xsegment, zsegment);
			v2 = base + this->vertexIndex(xsegment + 1, zsegment);
			v3 = base + this->vertexIndex(xsegment + 1, zsegment + 1);
			v4 = base + this->vertexIndex(xsegment, zsegment + 1);

			// These normals are here only to show lighting effect, they're not proper normals
			glNormal3fv(v1->normal);
			glVertex3fv(v1->position);

			glNormal3fv(v2->normal);
			glVertex3fv(v2->position);

			glNormal3fv(v4->normal);
			glVertex3fv(v4->position);


			glNormal3fv(v2->normal);
			glVertex3fv(v2->position);

			glNormal3fv(v3->normal);
			glVertex3fv(v3->position);

			glNormal3fv(v4->normal);
			glVertex3fv(v4->position);
		}
	}
		
	glEnd();

	if(this->normalsVisible) {
		this->drawNormals();
	}
}

float Water::heightAtPositionAndTime(const glm::vec3* position, float time) const {
	return this->waveX->valueForPositionAndTime(position->x, time)
		+ this->waveZ->valueForPositionAndTime(position->z, time);
}

glm::vec3 Water::normalAtPositionAndTime(const glm::vec3* position, float time) const {
	float dydx = this->waveX->derivativeForPositionAndTime(position->x, time);
	float dydz = this->waveZ->derivativeForPositionAndTime(position->z, time);

	// Calculate normals for both waves using method found on
	// http://goanna.cs.rmit.edu.au/~gl/teaching/i3d/2012/tutorial5.html
	auto vecx = glm::vec3(-dydx, 1, 0);
	auto vecz = glm::vec3(0, 1, -dydz);

	// Combine normal vectors
	auto result = glm::normalize(vecx + vecz);

	return result;
}

void Water::drawNormals() {
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


void Water::toggleNormals() {
	this->normalsVisible = !this->normalsVisible;
	printf("[Water] Normals visible: %d\n", this->normalsVisible);
}

Water* Water::doubleTesselationSafe() {
	if(this->segments * 2 <= Water::MAX_SEGMENTS) {
		this->segments *= 2;
	}

	this->resetData();
	printf("[Water] Current tesselation: %d\n", this->segments);

	return this;
}

Water* Water::halveTesselationSafe() {
	if(this->segments / 2 >= Water::MIN_SEGMENTS) {
		this->segments /= 2;
	}

	this->resetData();
	printf("[Water] Current tesselation: %d\n", this->segments);

	return this;
}