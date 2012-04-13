#include "precompile.h"
#include "water.h"

#include "../engine/utils.h"

Water::Water()
	: size(300, 300), segments(32),
	waveX((new Wave())->setAmplitude(4)->setLength(73)->setFrequency(0.7f)),
	waveZ((new Wave())->setAmplitude(4)->setLength(44)->setFrequency(0.5f)),
	time(0)
{

}

void Water::setTime(float value) {
	this->time = value;
}

void Water::draw() {
	float time = this->time;
	auto center = glm::vec3(0, 0, 0);

	glColor3f(0.0f, 0.2f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::vec2 segmentSize(this->size.x / this->segments,
		this->size.y / this->segments);

	
	glm::vec3 p1, p2, p3, p4;
	glm::vec3 n1, n2, n3, n4;

	auto start = center - glm::vec3(this->size.x * 0.5f, 0, -this->size.y * 0.5f);
	auto up = glm::vec3(0, 1, 0);

	glBegin(GL_TRIANGLES);
	for(int xsegment = 0; xsegment < this->segments; ++xsegment) {
		for(int zsegment = 0; zsegment < this->segments; ++zsegment) {
			p1 = start + glm::vec3(segmentSize.x * (xsegment + 0), 0.0f, -segmentSize.y * (zsegment + 0));
			p2 = start + glm::vec3(segmentSize.x * (xsegment + 1), 0.0f, -segmentSize.y * (zsegment + 0));
			p3 = start + glm::vec3(segmentSize.x * (xsegment + 1), 0.0f, -segmentSize.y * (zsegment + 1));
			p4 = start + glm::vec3(segmentSize.x * (xsegment + 0), 0.0f, -segmentSize.y * (zsegment + 1));

			p1.y = this->heightAtPositionAndTime(&p1, time);
			p2.y = this->heightAtPositionAndTime(&p2, time);
			p3.y = this->heightAtPositionAndTime(&p3, time);
			p4.y = this->heightAtPositionAndTime(&p4, time);

			n1 = this->normalAtPositionAndTime(&p1, time);
			n2 = this->normalAtPositionAndTime(&p2, time);
			n3 = this->normalAtPositionAndTime(&p3, time);
			n4 = this->normalAtPositionAndTime(&p4, time);

			// These normals are here only to show lighting effect, they're not proper normals
			glNormal3fv(glm::value_ptr(n1));
			glVertex3fv(glm::value_ptr(p1));

			glNormal3fv(glm::value_ptr(n2));
			glVertex3fv(glm::value_ptr(p2));

			glNormal3fv(glm::value_ptr(n4));
			glVertex3fv(glm::value_ptr(p4));


			glNormal3fv(glm::value_ptr(n2));
			glVertex3fv(glm::value_ptr(p2));

			glNormal3fv(glm::value_ptr(n3));
			glVertex3fv(glm::value_ptr(p3));

			glNormal3fv(glm::value_ptr(n4));
			glVertex3fv(glm::value_ptr(p4));
		}
	}
		
	glEnd();
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

void Water::toggleNormals() {
	this->normalsVisible = !this->normalsVisible;
	printf("[Water] Normals visible: %d\n", this->normalsVisible);
}

Water* Water::doubleTesselationSafe() {
	if(this->segments * 2 <= Water::MAX_SEGMENTS) {
		this->segments *= 2;
	}

	printf("[Water] Current tesselation: %d\n", this->segments);

	return this;
}

Water* Water::halveTesselationSafe() {
	if(this->segments / 2 >= Water::MIN_SEGMENTS) {
		this->segments /= 2;
	}

	printf("[Water] Current tesselation: %d\n", this->segments);

	return this;
}