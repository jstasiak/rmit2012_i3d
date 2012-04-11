#include "precompile.h"
#include "water.h"

#include "../engine/utils.h"

Water::Water()
	: size(300, 300), segments(100, 100),
	waveX((new Wave())->setAmplitude(4)->setLength(33)->setFrequency(0.7f)),
	waveZ((new Wave())->setAmplitude(4)->setLength(20)->setFrequency(0.5f))
{

}

Water::~Water() {
}

void Water::draw() {
	float time = millisecondsNow() / 1000.0f;
	auto center = glm::vec3(0, 0, 0);

	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0, 1, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::vec2 segmentSize(this->size.x / this->segments.x,
		this->size.y / this->segments.y);
	
	glm::vec3 p1, p2, p3, p4;
	auto start = center - glm::vec3(this->size.x * 0.5f, 0, -this->size.y * 0.5f);
	printf("start: %f %f %f\n", start.x, start.y, start.z);

	glBegin(GL_TRIANGLES);
	for(int xsegment = 0; xsegment < this->segments.x - 1; ++xsegment) {
		for(int zsegment = 0; zsegment < this->segments.y - 1; ++zsegment) {
			p1 = start + glm::vec3(segmentSize.x * (xsegment + 0), 0.0f, -segmentSize.y * (zsegment + 0));
			p2 = start + glm::vec3(segmentSize.x * (xsegment + 1), 0.0f, -segmentSize.y * (zsegment + 0));
			p3 = start + glm::vec3(segmentSize.x * (xsegment + 1), 0.0f, -segmentSize.y * (zsegment + 1));
			p4 = start + glm::vec3(segmentSize.x * (xsegment + 0), 0.0f, -segmentSize.y * (zsegment + 1));

			p1.y = this->heightAtPositionAndTime(&p1, time);
			p2.y = this->heightAtPositionAndTime(&p2, time);
			p3.y = this->heightAtPositionAndTime(&p3, time);
			p4.y = this->heightAtPositionAndTime(&p4, time);

			glVertex3fv(glm::value_ptr(p1));
			glVertex3fv(glm::value_ptr(p2));
			glVertex3fv(glm::value_ptr(p4));

			glVertex3fv(glm::value_ptr(p2));
			glVertex3fv(glm::value_ptr(p3));
			glVertex3fv(glm::value_ptr(p4));
		}
	}
		
	glEnd();
}

float Water::heightAtPositionAndTime(const glm::vec3* position, float time) const {
	return this->waveX->valueForPositionAndTime(position->x, time)
		+ this->waveZ->valueForPositionAndTime(position->z, time);
}