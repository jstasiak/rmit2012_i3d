#include "precompile.h"
#include "manager.h"

#include "../engine/application.h"
#include "../engine/command.h"
#include "../engine/gameobject/gameobjectset.h"
#include "../engine/gameobject/basegameobject.h"
#include "../engine/gameobject/transform.h"
#include "../engine/scene.h"
#include "../engine/gameobject/ball.h"
#include "../engine/gameobject/rigidbody.h"
#include "../engine/font.h"
#include "../engine/gameobject/camera.h"
#include "../engine/utils.h"

#include "ship.h"
#include "water.h"


Manager::Manager()
	:wireframe(false),
	axes(Manager::WorldOrigin),
	font()
{

}

void Manager::start() {
	auto app = this->gameObject->getApplication();
	auto cs = app->getCommandSystem();

	auto fontFile = app->getDataDirectory() + std::string("/fonts/Comfortaa_Regular.ttf");
	this->font = std::shared_ptr< Font >(new Font(fontFile));

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

	cs->registerCommand("fire", [this, getShip](command_parameters parameters) {
		assert(parameters.size() >= 2);
		auto shipId = parameters.at(0);
		auto side = parameters.at(1);
		getShip(shipId)->fire(side);
	});

	cs->registerCommand("reset", [this](command_parameters parameters) {
		this->resetGame();
	});

	this->createShips();
}


void Manager::resetGame() {
	if(this->getState() != Manager::STATE_RUNNING) {
		this->clearShips();
		this->createShips();
	}
}

void Manager::clearShips() {
	auto ships = this->getShips();
	BOOST_FOREACH(auto ship, ships) {
		ship->destroy();
	}
}

void Manager::createShips() {
	auto createShip = [](std::string name, glm::vec3 position) -> std::shared_ptr< BaseGameObject > {
		auto ship = std::make_shared<Ship>();
		ship->setName(name);
		auto components = ship->getComponents();
		auto transform = components->getSingleByClass< Transform >();
		transform->setPosition(position);

		auto body = std::make_shared< RigidBody >();
		body->setRadius(10);
		body->setGravityEnabled(false);
		
		components->add(body);

		return ship;
	};

	auto ship1 = createShip("ship1", glm::vec3(80, 0, 80));
	auto ship2 = createShip("ship2", glm::vec3(-80, 0, -80));

	auto gos = this->gameObject->getGameObjectSet().lock();
	auto scene = gos->getOwner();

	scene->add(ship1);
	scene->add(ship2);


	auto c1 = std::make_shared<Camera>();
	auto c2 = std::make_shared<Camera>();
	c1->setNormalizedRect(Rectf(0, 0, 0.5, 1));
	c2->setNormalizedRect(Rectf(0.5, 0.0, 0.5, 1));
	c2->setDepth(1);

	c1->setOwnerObject(ship1);
	c1->setTrackedObject(ship2);
	
	c2->setOwnerObject(ship2);
	c2->setTrackedObject(ship1);

	scene->add(c1);
	scene->add(c2);
}

Manager::GameState Manager::getState() {
	auto gos = this->gameObject->getGameObjectSet().lock();
	auto ship1 = gos->getSingleByName<Ship>("ship1");
	auto ship2 = gos->getSingleByName<Ship>("ship2");

	GameState state;

	if(ship1->isDead()) {
		if(ship2->isDead()) {
			state = Manager::STATE_DRAW;
		}
		else {
			state = Manager::STATE_PLAYER2_WON;
		}
	}
	else {
		if(ship2->isDead()) {
			state = Manager::STATE_PLAYER1_WON;
		}
		else {
			state = Manager::STATE_RUNNING;
		}
	}

	return state;
}



void Manager::update(std::shared_ptr<FrameEventArgs> args) {
	glPolygonMode(GL_FRONT, this->wireframe ? GL_LINE : GL_FILL);
}

void Manager::onGui() {
	BaseComponent::onGui();

	auto app = this->gameObject->getApplication();
	auto ss = app->getScreenSize();

	float fps = app->getCurrentDrawFps();
	auto text = (boost::format("Draw FPS: %.1f") % fps).str();
	this->font->drawText(glm::vec2(0, ss.y), 15, text);

	auto state = this->getState();

	std::string message("");

	auto center = glm::vec2(ss.x * 1.0f / 2, ss.y * 1.0f / 2);

	if(state != Manager::STATE_RUNNING) {
		if(state == Manager::STATE_DRAW) {
			message += std::string("DRAW");
		}
		else {
			message += (boost::format("PLAYER %1% WON") % (state == Manager::STATE_PLAYER1_WON ? 1 : 2 )).str();
		}

		this->font->drawText(center, 30, message);

		message = std::string("Press ENTER to reset");
		this->font->drawText(center + glm::vec2(0, -40), 30, message);
	}
}

std::list< std::shared_ptr< Ship > > Manager::getShips() {
	auto gos = this->gameObject->getGameObjectSet().lock();
	return gos->getMultipleByClass<Ship>();
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