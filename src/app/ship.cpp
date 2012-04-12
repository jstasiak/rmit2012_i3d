#include "precompile.h"
#include "ship.h"


Ship::Ship() : position(0, 0, 0) {
	this->mesh = objMeshLoad("models/galleon.obj");
}

void Ship::draw() const {
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