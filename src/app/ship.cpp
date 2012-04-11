#include "precompile.h"
#include "ship.h"


Ship::Ship() {
	this->mesh = objMeshLoad("models/galleon.obj");
}

void Ship::draw() const {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh->numIndices; ++i) {
		unsigned int index = mesh->indices[i];
		float* vert = (float*)((char*)mesh->vertices + index * mesh->stride);
		float* norm = (float*)((char*)vert + mesh->normalOffset);
		if (mesh->hasNormals)
			glNormal3fv(norm);
		glVertex3fv(vert);
	}
	glEnd();
}

Ship::~Ship() {
	if(this->mesh) {
		objMeshFree(&this->mesh);
		this->mesh = 0;
	}
}
