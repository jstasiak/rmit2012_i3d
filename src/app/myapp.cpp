#include "precompile.h"
#include "myapp.h"

#include <SDL_opengl.h>

#include "../engine/command.h"
#include "../engine/utils.h"
#include "ship.h"
#include "water.h"

using namespace std;


MyApp::MyApp()
	:
	wireframe(false),
	axes(MyApp::WorldOrigin)
{
}

MyApp::~MyApp() {

}

void MyApp::initialize() {
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_SCISSOR_TEST);


	this->bindings[SDLK_a] = "toggle_axes";
	this->bindings[SDLK_EQUALS] = "increase_water_tesselation";
	this->bindings[SDLK_MINUS] = "decrease_water_tesselation";
	this->bindings[SDLK_n] = "toggle_water_normals";
	this->bindings[SDLK_w] = "toggle_wireframe";
	this->bindings[SDLK_UP] = "+accel";
	this->bindings[SDLK_DOWN] = "+deccel";
	this->bindings[SDLK_LEFT] = "+left";
	this->bindings[SDLK_RIGHT] = "+right";

	auto r = Registry::getSharedInstance();
	auto water = r->create<BaseGameObject>("Water");
	auto ship = r->create<BaseGameObject>("Ship");

	this->gameObjectSet->add(water);
	this->gameObjectSet->add(ship);

	this->initializeCommands();
}

void MyApp::initializeCommands() {
	auto cs = this->getCommandSystem();

	cs->registerCommand("toggle_wireframe", [this](command_parameters parameters) {
		this->toggleWireframe();
	});

	cs->registerCommand("toggle_axes", [this](command_parameters parameters) {
		this->toggleAxes();
	});
}

void MyApp::beforeDraw(std::shared_ptr<FrameEventArgs> args) {
	Application::beforeDraw(args);

	// Switch between wireframe and fill mode
	glPolygonMode(GL_FRONT, this->wireframe ? GL_LINE : GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(this->axes == MyApp::WorldOrigin) {
		drawAxes(50.0f);
	}

	auto ship = this->gameObjectSet->getSingleByClass<Ship>();

	// Enable lights in camera space only so light position is ok
	this->enableLights();

	if(this->axes == MyApp::WaterOrigin) {
		drawAxes(50);
	}
}

void MyApp::enableLights() {
	glEnable(GL_LIGHTING);

	float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float position[] = {0.0f, 30.0f, -100.0f, 1.0f};

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

	float shininess[] = { 64.0f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, black);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	glEnable(GL_LIGHT0);
}

void MyApp::toggleWireframe() {
	this->wireframe = !this->wireframe;
}

void MyApp::toggleAxes() {
	auto ship = this->gameObjectSet->getSingleByClass<Ship>();

	if(this->axes == MyApp::WorldOrigin) {
		this->axes = MyApp::WaterOrigin;
	}
	else if(this->axes == MyApp::WaterOrigin) {
		this->axes = MyApp::ShipOrigin;
		ship->setAxes(Ship::Draw);
	}
	else if(this->axes == MyApp::ShipOrigin) {
		this->axes = MyApp::ShipOriginWithRotation;
		ship->setAxes(Ship::DrawWithRotation);
	}
	else if(this->axes == MyApp::ShipOriginWithRotation) {
		this->axes = MyApp::WorldOrigin;
		ship->setAxes(Ship::DontDraw);
	}
}