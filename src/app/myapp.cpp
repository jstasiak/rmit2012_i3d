#include "precompile.h"
#include "myapp.h"

#include <SDL_opengl.h>

#include "../engine/command.h"
#include "../engine/utils.h"
#include "ship.h"
#include "water.h"

using namespace std;


MyApp::MyApp()
{
}

MyApp::~MyApp() {

}

void MyApp::initialize() {

	this->initializeCommands();
}


void MyApp::beforeDraw(std::shared_ptr<FrameEventArgs> args) {
	Application::beforeDraw(args);
}

