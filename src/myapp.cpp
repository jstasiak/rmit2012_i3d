#include "myapp.h"

#include <cmath>
#include <cstdio>
#include <SDL_opengl.h>

#include "engine/utils.h"

void MyApp::update( FrameEventArgs* args ) {
}

void MyApp::draw( FrameEventArgs* args ) {
	glClearColor(0.0f, 0.0f, abs(sin(millisecondsNow() / 1000.0f)), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	printf("Draw %f seconds\n", args->getSeconds());
}
