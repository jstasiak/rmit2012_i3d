#include "precompile.h"
#include "manager.h"

#include "../engine/application.h"
#include "../engine/command.h"
#include "../engine/gameobject/gameobjectset.h"
#include "../engine/gameobject/basegameobject.h"
#include "../engine/gameobject/transform.h"
#include "../engine/scene.h"

#include "ship.h"
#include "water.h"

REGISTER(Manager);

Manager::Manager()
	:wireframe(false),
	axes(Manager::WorldOrigin)
{

}

void Manager::start() {
	auto app = this->gameObject->getApplication();
	auto cs = app->getCommandSystem();

	cs->registerCommand("toggle_wireframe", [this](command_parameters parameters) {
		this->toggleWireframe();
	});

	cs->registerCommand("toggle_axes", [this](command_parameters parameters) {
		this->toggleAxes();
	});

	auto getShip = [this](std::string id) -> std::shared_ptr<Ship> {
		auto gos = this->getGameObject()->getGameObjectSet().lock();
		auto name = std::string("ship") + id;
		return gos->getSingleByName<Ship>(name);
	};

	// Acceleration commands
	auto stopAcceleration = [this, getShip](command_parameters parameters) {
		auto shipId = parameters.front();
		getShip(shipId)->stopAcceleration();
	};
	cs->registerCommand("+accel", [this, getShip](command_parameters parameters) {
		auto shipId = parameters.front();
		getShip(shipId)->startAcceleration();
	});

	cs->registerCommand("-accel", stopAcceleration);

	cs->registerCommand("+deccel", [this, getShip](command_parameters parameters) {
		auto shipId = parameters.front();
		getShip(shipId)->startDecceleration();
	});

	cs->registerCommand("-deccel", stopAcceleration);

	// Turning commands
	cs->registerCommand("+left", [this, getShip](command_parameters parameters) {
		auto shipId = parameters.front();
		getShip(shipId)->startTurningLeft();
	});

	cs->registerCommand("-left", [this, getShip](command_parameters parameters) {
		auto shipId = parameters.front();
		getShip(shipId)->stopTurningLeft();
	});

	cs->registerCommand("+right", [this, getShip](command_parameters parameters) {
		auto shipId = parameters.front();
		getShip(shipId)->startTurningRight();
	});

	cs->registerCommand("-right", [this, getShip](command_parameters parameters) {
		auto shipId = parameters.front();
		getShip(shipId)->stopTurningRight();
	});

	cs->registerCommand("test", [this, getShip](command_parameters parameters) {
		try {
			auto ship = getShip("test");
			printf("ship exists, destroying...\n");
			ship->destroy();
		}
		catch(const DoesNotExist& e) {
			printf("there's no ship, creating new\n");
			auto scene = this->getGameObject()->getGameObjectSet().lock()->getOwner();
			auto r = Registry::getSharedInstance();
			auto newShip = r->create<Ship>("Ship");
			newShip->getComponents()->getSingleByClass<Transform>()->setPosition(glm::vec3(0, 100, 0));
			newShip->setUseWaterLevel(false);
			newShip->setName("shiptest");
			newShip->getComponents()->add(r->create<BaseComponent>("RigidBody"));
			scene->add(newShip);
		}
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