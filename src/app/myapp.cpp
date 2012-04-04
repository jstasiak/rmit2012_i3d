#include "precompile.h"
#include "myapp.h"

#include <cmath>
#include <cstdio>
#include <SDL_opengl.h>

#include "../engine/utils.h"
#include "water.h"

void MyApp::update(FrameEventArgs* args) {
}

void MyApp::draw(FrameEventArgs* args) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.33f, 0.01f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(-35.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0, 15.0f, -15.0f);

	glClearColor(0, 0, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	this->water->draw();
}

MyApp::MyApp() : water(new Water()) {
	
}
