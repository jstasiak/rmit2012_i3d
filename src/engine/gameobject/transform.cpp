#include "precompile.h"
#include "transform.h"

using namespace std;

Transform::Transform()
	:
	matrix()
{
}


glm::vec3 Transform::getPosition() const {
	auto column = glm::column(this->matrix, 3);
	return column.swizzle(glm::X, glm::Y, glm::Z);
}

void Transform::setPosition(glm::vec3 value) {
	this->translateGlobal(value + -this->getPosition());
}

glm::mat4x4 Transform::getMatrix() {
	return this->matrix;
}

glm::vec3 Transform::getForward() {
	auto column = glm::column(this->matrix, 2);
	glm::normalize(column);
	glm::vec3 column3 = column.swizzle(glm::X, glm::Y, glm::Z);
	return -column3;
}

glm::vec3 Transform::getUp() {
	auto column = glm::column(this->matrix, 1);
	glm::normalize(column);
	return column.swizzle(glm::X, glm::Y, glm::Z);
}

glm::vec3 Transform::getRight() {
	auto column = glm::column(this->matrix, 0);
	glm::normalize(column);
	return column.swizzle(glm::X, glm::Y, glm::Z);
}

void Transform::translateGlobal(glm::vec3 value) {
	glm::mat4x4 mat;
	mat = glm::translate(mat, value);
	this->matrix = mat * this->matrix;
}

void Transform::translateLocal(glm::vec3 value) {
	this->matrix = glm::translate(this->matrix, value);
}

void Transform::rotateAroundGlobal(glm::vec3 axis, float degrees) {
	glm::mat4x4 mat;
	mat = glm::rotate(mat, degrees, axis);
	this->matrix = mat * this->matrix;
}

void Transform::rotateAroundLocal(glm::vec3 axis, float degrees) {
	this->matrix = glm::rotate(this->matrix, degrees, axis);
}