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

	this->bindings[SDLK_a] = "toggle_axes";
	this->bindings[SDLK_EQUALS] = "increase_water_tesselation";
	this->bindings[SDLK_MINUS] = "decrease_water_tesselation";
	this->bindings[SDLK_n] = "toggle_normals";
	this->bindings[SDLK_UP] = "+accel";
	this->bindings[SDLK_DOWN] = "+deccel";
	this->bindings[SDLK_LEFT] = "+left";
	this->bindings[SDLK_RIGHT] = "+right";
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

	auto shipPosition = this->ship->getPosition();
	float height = this->water->heightAtPositionAndTime(shipPosition, args->getTotalSeconds());
	shipPosition->y = height;
	this->ship->draw();

	drawAxes(50);
}

void MyApp::applyCameraTransform() const {
	auto position = this->ship->getPosition();
	gluLookAt(0, 100, 130,
		position->x, 0, position->z,
		0, 1, 0);
}