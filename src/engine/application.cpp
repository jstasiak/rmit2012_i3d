#include "precompile.h"
#include "application.h"

#include "frameeventargs.h"
#include "utils.h"

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
			this->doUpdate(FrameEventArgs::createFromSecondsAndTotalSeconds(this->updateEverySeconds, now));
		}

		if(drawDt >= this->drawEverySeconds) {
			this->doDraw(FrameEventArgs::createFromSecondsAndTotalSeconds(drawDt, now));
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

void Application::doUpdate(FrameEventArgs* args) {
	assert(args);
	this->update(args);
}

void Application::doDraw(FrameEventArgs* args) {
	assert(args);

	this->draw(args);
	SDL_GL_SwapBuffers();
}

void Application::update( FrameEventArgs* args )
{

}

void Application::draw( FrameEventArgs* args )
{

}
