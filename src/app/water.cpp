#include "water.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/multiple.hpp>
#include <SDL_opengl.h>

Water::Water()
	: size(new glm::vec2(20, 20)),
	segments(new glm::ivec2(10, 10)) {

}

Water::~Water() {
	delete this->size;
	delete this->segments;

	this->size = 0;
	this->segments = 0;
}

void Water::draw() {
	auto center = glm::vec2(0, 0);
	auto p1 = (center + glm::vec2(-0.5, -0.5) * *this->size).swizzle(glm::X, glm::Y, glm::X);
	auto p2 = (center + glm::vec2(0.5, -0.5) * *this->size).swizzle(glm::X, glm::Y, glm::X);
	auto p3 = (center + glm::vec2(0.5, 0.5) * *this->size).swizzle(glm::X, glm::Y, glm::X);
	auto p4 = (center + glm::vec2(-0.5, 0.5) * *this->size).swizzle(glm::X, glm::Y, glm::X);

	p1.z = 0;
	p2.z = 0;
	p3.z = 0;
	p4.z = 0;

	glColor3f(1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glBegin(GL_TRIANGLES);
		glVertex2f(-5, -5);
		glVertex2f(5, -5);
		glVertex2f(0, 5);
	
		glVertex3f(p1.x, p1.y, 0.0f);
		glVertex3f(p2.x, p2.y, 0.0f);
		glVertex3f(p4.x, p4.y, 0.0f);

		glVertex3f(p2.x, p2.y, 0.0f);
		glVertex3f(p3.x, p3.y, 0.0f);
		glVertex3f(p4.x, p4.y, 0.0f);
	glEnd();
}

