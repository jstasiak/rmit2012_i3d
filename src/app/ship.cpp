#include "precompile.h"
#include "ship.h"

const float Ship::MIN_VELOCITY = 0.0f;
const float Ship::MAX_VELOCITY = 30.0f;
const float Ship::ACCELERATION = 15.0f;

Ship::Ship() : position(0, 0, 0) {
	this->mesh = objMeshLoad("models/galleon.obj");
}

void Ship::startAcceleration() {
	this->currentAcceleration = Ship::ACCELERATION;
}

void Ship::startDecceleration() {
	this->currentAcceleration = -Ship::ACCELERATION;
}

void Ship::stopAcceleration() {
	this->currentAcceleration = 0.0f;
}

void Ship::update(FrameEventArgs* args) {
	if(this->currentAcceleration != 0) {
		auto dv = this->currentAcceleration * args->getSeconds();
		this->velocity = std::min(Ship::MAX_VELOCITY,
			std::max(Ship::MIN_VELOCITY,
				this->velocity + dv
			)
		);
	}

	if(this->velocity > 0) {
		auto dx = glm::vec3(0, 0, 1) * this->velocity * args->getSeconds();
		printf("Ship dx: %f %f %f\n", dx.x, dx.y, dx.z);
		this->position += dx;
	}
}

void Ship::draw(FrameEventArgs* args) {
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);

	float scale = 0.4f;
	glScalef(scale, scale, scale);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh->numIndices; ++i) {
		unsigned int index = mesh->indices[i];
		float* vertex = (float*)((char*)mesh->vertices + index * mesh->stride);
		float* normal = (float*)((char*)vertex + mesh->normalOffset);
		if (mesh->hasNormals)
			glNormal3fv(normal);

		glVertex3fv(vertex);
	}
	glEnd();

	glPopMatrix();
}

Ship::~Ship() {
	if(this->mesh) {
		objMeshFree(&this->mesh);
		this->mesh = 0;
	}
}

glm::vec3* Ship::getPosition() {
	return &(this->position);
}

Ship* Ship::setPosition(const glm::vec3* value) {
	this->position = *value;
	return this;
}