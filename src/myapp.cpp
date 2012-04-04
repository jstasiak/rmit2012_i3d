#include "myapp.h"

#include <cmath>
#include <cstdio>
#include <SDL_opengl.h>

#include "engine/utils.h"

void MyApp::update(FrameEventArgs* args) {
}

void MyApp::draw(FrameEventArgs* args) {
	float data[] = {
		5.0f, -3.0f, -7.0f,
		-5.0f, -3.0f, -7.0f,
		0.0f, 5.0f, -7.0f
	};

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.33f, 0.01f, 500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glClearColor(0.0f, 0.0f, abs(sin(millisecondsNow() / 1000.0f)), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, data);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

MyApp::MyApp() {
	
}
