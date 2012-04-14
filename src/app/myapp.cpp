#include "precompile.h"
#include "myapp.h"

#include <SDL_opengl.h>

#include "../engine/utils.h"
#include "ship.h"
#include "water.h"

MyApp::MyApp()
	: water(new Water()),
	ship(new Ship(&(*this->water))),
	wireframe(false),
	axes(MyApp::WorldOrigin)
{
}

MyApp::~MyApp() {

}

void MyApp::initialize() {
	this->initializeGraphics();
	this->initializeCommands();
	this->initializeKeyBindings();
}

void MyApp::initializeGraphics() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.33f, 0.01f, 500.0f);
	glClearColor(0, 0, 0, 1.0f);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void MyApp::initializeCommands() {
	auto& cs = this->commandSystem;

	cs->registerCommand("increase_water_tesselation", [this]() {
		this->water->doubleTesselationSafe();
	});

	cs->registerCommand("decrease_water_tesselation", [this]() {
		this->water->halveTesselationSafe();
	});

	cs->registerCommand("toggle_water_normals", [this]() {
		this->water->toggleNormals();
	});

	// Acceleration commands
	auto stopAcceleration = [this]() {
		this->ship->stopAcceleration();
	};
	cs->registerCommand("+accel", [this]() {
		this->ship->startAcceleration();
	});

	cs->registerCommand("-accel", stopAcceleration);

	cs->registerCommand("+deccel", [this]() {
		this->ship->startDecceleration();
	});

	cs->registerCommand("-deccel", stopAcceleration);

	// Turning commands
	cs->registerCommand("+left", [this]() {
		this->ship->startTurningLeft();
	});

	cs->registerCommand("-left", [this]() {
		this->ship->stopTurningLeft();
	});

	cs->registerCommand("+right", [this]() {
		this->ship->startTurningRight();
	});

	cs->registerCommand("-right", [this]() {
		this->ship->stopTurningRight();
	});


	cs->registerCommand("toggle_wireframe", [this]() {
		this->toggleWireframe();
	});

	cs->registerCommand("toggle_axes", [this]() {
		this->toggleAxes();
	});
}

void MyApp::initializeKeyBindings() {
	this->bindings[SDLK_a] = "toggle_axes";
	this->bindings[SDLK_EQUALS] = "increase_water_tesselation";
	this->bindings[SDLK_MINUS] = "decrease_water_tesselation";
	this->bindings[SDLK_n] = "toggle_water_normals";
	this->bindings[SDLK_w] = "toggle_wireframe";
	this->bindings[SDLK_UP] = "+accel";
	this->bindings[SDLK_DOWN] = "+deccel";
	this->bindings[SDLK_LEFT] = "+left";
	this->bindings[SDLK_RIGHT] = "+right";
}

void MyApp::update(FrameEventArgs* args) {
	this->water->update(args);
	this->ship->update(args);
}

void MyApp::draw(FrameEventArgs* args) {
	glPolygonMode(GL_FRONT, this->wireframe ? GL_LINE : GL_FILL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if(this->axes == MyApp::WorldOrigin) {
		drawAxes(50.0f);
	}
	this->applyCameraTransform();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->enableLights();

	auto zero = glm::vec3(0, 0, 0);
	this->water->draw(args);

	auto shipPosition = this->ship->getPosition();
	float height = this->water->heightAtPositionAndTime(shipPosition, args->getTotalSeconds());
	shipPosition->y = height;
	this->ship->draw(args);

	if(this->axes == MyApp::WaterOrigin) {
		drawAxes(50);
	}
}

void MyApp::applyCameraTransform() const {
	auto position = this->ship->getPosition();
	gluLookAt(0, 100, 100,
		position->x, 0, position->z,
		0, 1, 0);
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
	if(this->axes == MyApp::WorldOrigin) {
		this->axes = MyApp::WaterOrigin;
	}
	else if(this->axes == MyApp::WaterOrigin) {
		this->axes = MyApp::ShipOrigin;
		this->ship->setAxes(Ship::Draw);
	}
	else if(this->axes == MyApp::ShipOrigin) {
		this->axes = MyApp::ShipOriginWithRotation;
		this->ship->setAxes(Ship::DrawWithRotation);
	}
	else if(this->axes == MyApp::ShipOriginWithRotation) {
		this->axes = MyApp::WorldOrigin;
		this->ship->setAxes(Ship::DontDraw);
	}
}