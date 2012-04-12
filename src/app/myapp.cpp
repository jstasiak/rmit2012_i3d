#include "precompile.h"
#include "myapp.h"

#include <cmath>
#include <cstdio>
#include <SDL_opengl.h>

#include "../engine/utils.h"
#include "ship.h"
#include "water.h"

MyApp::MyApp() : water(new Water()), ship(new Ship()) {

}

MyApp::~MyApp() {

}

void MyApp::initialize() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.33f, 0.01f, 500.0f);
	glClearColor(0, 0, 0, 1.0f);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
}

void MyApp::update(FrameEventArgs* args) {
}

void MyApp::draw(FrameEventArgs* args) {
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	this->applyCameraTransform();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto zero = glm::vec3(0, 0, 0);
	this->water->draw();

	float height = this->water->heightAtPositionAndTime(&zero, args->getTotalSeconds());
	glPushMatrix();
	glTranslatef(0, height, 0);
	glScalef(0.3f, 0.4f, 0.4f);
	this->ship->draw();
	glPopMatrix();

	drawAxes(50);
}

void MyApp::applyCameraTransform() const {
	gluLookAt(0, 100, 230, 0, 0, 0, 0, 1, 0);
}