#include "precompile.h"
#include "water.h"

#include "../engine/utils.h"

Water::Water()
	: size(300, 300), segments(50, 50),
	waveX((new Wave())->setAmplitude(4)->setLength(73)->setFrequency(0.7f)),
	waveZ((new Wave())->setAmplitude(4)->setLength(44)->setFrequency(0.5f))
{

}

Water::~Water() {
}

void Water::draw() {
	float time = millisecondsNow() / 1000.0f;
	auto center = glm::vec3(0, 0, 0);

	glColor3f(0.0f, 0.2f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glm::vec2 segmentSize(this->size.x / this->segments.x,
		this->size.y / this->segments.y);
	
	glm::vec3 p1, p2, p3, p4;
	glm::vec3 n1, n2, n3, n4;

	auto start = center - glm::vec3(this->size.x * 0.5f, 0, -this->size.y * 0.5f);
	auto up = glm::vec3(0, 1, 0);

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
	// Add PI/2 phase difference so we get cos() instead of sin()
	float phase = M_PI / 2.0f;
	this->waveX->setPhase(this->waveX->getPhase() + phase);
	this->waveZ->setPhase(this->waveZ->getPhase() + phase);

	// Compute derivatives
	float valx = this->waveX->valueForPositionAndTime(position->x, time) * this->waveX->getK();
	float valz = this->waveZ->valueForPositionAndTime(position->z, time) * this->waveZ->getK();

	// Set wave phase to original state
	this->waveX->setPhase(this->waveX->getPhase() - phase);
	this->waveZ->setPhase(this->waveZ->getPhase() - phase);

	// Calculate normals for both waves
	auto vecx = glm::vec3(-valx, 1, 0);
	auto vecz = glm::vec3(0, 1, -valz);

	// Combine normal vectors
	auto result = glm::normalize(vecx + vecz);

	return result;
}