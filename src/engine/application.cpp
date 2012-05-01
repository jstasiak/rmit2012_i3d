#include "precompile.h"
#include "application.h"

#include "frameeventargs.h"
#include "utils.h"

Application::Application()
	: commandSystem(new CommandSystem()), bindings() {
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
	assert(result == 0);

	auto surface = SDL_SetVideoMode(800, 600, 32, SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE);
	assert(surface);

	double last = millisecondsNow() / 1000.0f;
	float updateDt = 0.0f;
	float drawDt = 0.0f;
	double now = 0.0;
	double dt = 0.0;

	bool running = true;

	SDL_Event event;

	this->doInitialize();

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				this->onKeyDown(&event.key);
				break;
			case SDL_KEYUP:
				this->onKeyUp(&event.key);
				break;
			}
		}

		now = millisecondsNow() / 1000.0f;
		dt = now - last;
		last = now;
		updateDt += dt;
		drawDt += dt;

		while(updateDt >= this->updateEverySeconds) {
			//FIXME: HACK: develop smarter system of update managing
			updateDt -= this->updateEverySeconds;
			auto args = boost::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(this->updateEverySeconds, now));
			this->doUpdate(args);
		}

		if(drawDt >= this->drawEverySeconds) {
			auto args = boost::shared_ptr<FrameEventArgs>(
				FrameEventArgs::createFromSecondsAndTotalSeconds(drawDt, now));
			this->doDraw(args);
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

void Application::doInitialize() {
	this->bindings[SDLK_q] = "quit";
	this->bindings[SDLK_ESCAPE] = "quit";

	this->commandSystem->registerCommand("quit", [this]() {
		this->quit();
	});

	this->initialize();
}

void Application::doUpdate(boost::shared_ptr<FrameEventArgs> args) {
	boost::shared_ptr<FrameEventArgs> sa(args);
	assert(args);
	this->update(sa);
}

void Application::doDraw(boost::shared_ptr<FrameEventArgs> args) {
	assert(args);

	this->draw(args);
	SDL_GL_SwapBuffers();
}

void Application::initialize() {

}

void Application::onKeyDown(const SDL_KeyboardEvent* event) {
	auto iterator = this->bindings.find(event->keysym.sym);
	if(iterator != this->bindings.end()) {
		auto commandName = (*iterator).second;
		this->commandSystem->safeExecuteCommand(commandName);
	}
}

void Application::onKeyUp(const SDL_KeyboardEvent* event) {
	auto iterator = this->bindings.find(event->keysym.sym);
	if(iterator != this->bindings.end()) {
		std::string commandName = (*iterator).second;
		if(commandName.at(0) == '+') {
			commandName.at(0) = '-';
			this->commandSystem->safeExecuteCommand(commandName);
		}
	}
}

void Application::update(boost::shared_ptr<FrameEventArgs> args) {

}

void Application::draw(boost::shared_ptr<FrameEventArgs> args) {

}

/**
 * Send SDL_QUIT event to message loop
 */
void Application::quit() {
	SDL_Event event;
	event.type = SDL_QUIT;
	SDL_PushEvent(&event);
}