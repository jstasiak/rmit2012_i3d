#include "precompile.h"
#include "manager.h"

#include "../engine/application.h"
#include "../engine/command.h"
#include "../engine/gameobject/gameobjectset.h"
#include "../engine/gameobject/basegameobject.h"
#include "ship.h"
#include "water.h"

REGISTER(Manager);

Manager::Manager()
	:wireframe(false),
	axes(Manager::WorldOrigin)
{

}

void Manager::start() {
	auto app = this->gameObject->getGameObjectSet().lock()->getOwner();
	auto cs = app->getCommandSystem();

	cs->registerCommand("toggle_wireframe", [this](command_parameters parameters) {
		this->toggleWireframe();
	});

	cs->registerCommand("toggle_axes", [this](command_parameters parameters) {
		this->toggleAxes();
	});
}

void Manager::update(std::shared_ptr<FrameEventArgs> args) {
	glPolygonMode(GL_FRONT, this->wireframe ? GL_LINE : GL_FILL);
}


void Manager::toggleWireframe() {
	this->wireframe = !this->wireframe;
}

void Manager::toggleAxes() {
	auto objectSet = this->gameObject->getGameObjectSet().lock();

	auto ship = objectSet->getSingleByClass<Ship>();
	auto water = objectSet->getSingleByClass<Water>();

	water->setAxesVisible(false);

	if(this->axes == Manager::WorldOrigin) {
		this->axes = Manager::WaterOrigin;
		water->setAxesVisible(true);
	}
	else if(this->axes == Manager::WaterOrigin) {
		this->axes = Manager::ShipOrigin;
		ship->setAxes(Ship::Draw);
	}
	else if(this->axes == Manager::ShipOrigin) {
		this->axes = Manager::ShipOriginWithRotation;
		ship->setAxes(Ship::DrawWithRotation);
	}
	else if(this->axes == Manager::ShipOriginWithRotation) {
		this->axes = Manager::WorldOrigin;
		ship->setAxes(Ship::DontDraw);
	}
}