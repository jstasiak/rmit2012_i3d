#include "application.h"

#include <cassert>
#include <cmath>

#include <SDL_opengl.h>
#include <SDL.h>

#include "frameeventargs.h"
#include "../utils.h"

Application::Application() {
	setUpdateFps(77);
	setDrawFps(60);
}

Application::~Application() {
	SDL_Quit();
}

void Application::applyCommandlineParameters(int argc, char** argv) {

}

int Application::run() {
	int result;

	result = SDL_Init(SDL_INIT_EVERYTHING);
	if(result != 0) {
		throw 0;
	}

	auto surface = SDL_SetVideoMode(800, 600, 32, SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE);
	if(!surface) {
		throw 0;
	}

	double last = millisecondsNow() / 1000.0f;
	float updateDt = 0.0f;
	float drawDt = 0.0f;
	double now = 0.0;
	double dt = 0.0;

	bool running = true;

	SDL_Event event;

	while(running) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT || event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
				running = false;
			}
		}

		now = millisecondsNow() / 1000.0f;
		dt = now - last;
		last = now;
		updateDt += dt;
		drawDt += dt;

		while(updateDt >= this->updateEverySeconds) {
			updateDt -= this->updateEverySeconds;
			this->update(FrameEventArgs::createFromSeconds(this->updateEverySeconds));
		}

		if(drawDt >= this->drawEverySeconds) {
			this->draw(FrameEventArgs::createFromSeconds(drawDt));
			SDL_GL_SwapBuffers();
			drawDt = 0.0;
		}
	}
	return 0;
}

void Application::setUpdateFps( int value )
{
	assert(value > 0);
	this->updateFps = value;
	this->updateEverySeconds = 1.0f / this->updateFps;
}

void Application::setDrawFps( int value )
{
	assert(value > 0);
	this->drawFps = value;
	this->drawEverySeconds = 1.0f / this->drawFps;
}

void Application::update( FrameEventArgs* args )
{
	assert(args);
	printf("Update %f seconds\n", args->getSeconds());
}

void Application::draw( FrameEventArgs* args )
{
	assert(args);
	glClearColor(0.0f, 0.0f, abs(sin(millisecondsNow() / 1000.0f)), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	printf("Draw %f seconds\n", args->getSeconds());
}
