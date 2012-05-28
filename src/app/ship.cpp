#include "precompile.h"
#include "ship.h"

#include "../engine/application.h"
#include "../engine/command.h"
#include "../engine/utils.h"


#include "../engine/gameobject/gameobjectset.h"
#include "../engine/gameobject/transform.h"

#include "../engine/gameobject/ball.h"
#include "../engine/gameobject/rigidbody.h"
#include "../engine/scene.h"

#include "projectile.h"
#include "terrain.h"
#include "water.h"

using namespace std;

const float Ship::MIN_VELOCITY = 0.0f;
const float Ship::MAX_VELOCITY = 45.0f;
const float Ship::ACCELERATION = 30.0f;
const float Ship::TURNING_SPEED_DEGREES_PER_SECOND = 30.0f;

Ship::Ship()
	:
	currentAcceleration(0.0f),
	currentTurningSpeedDegreesPerSecond(0.0f),
	water(),
	mesh(0),
	useWaterLevel(true),
	hp(100)
{
}

void Ship::startAcceleration() {
	this->currentAcceleration = Ship::ACCELERATION;
}

void Ship::startDecceleration() {
	this->currentAcceleration = -Ship::ACCELERATION;
}

void Ship::stopAcceleration() {
	this->currentAcceleration = 0.0f;
}

void Ship::startTurningLeft() {
	this->currentTurningSpeedDegreesPerSecond = Ship::TURNING_SPEED_DEGREES_PER_SECOND;
}

void Ship::stopTurningLeft() {
	if(this->currentTurningSpeedDegreesPerSecond > 0) {
		this->currentTurningSpeedDegreesPerSecond = 0;
	}
}


void Ship::startTurningRight() {
	this->currentTurningSpeedDegreesPerSecond = -Ship::TURNING_SPEED_DEGREES_PER_SECOND;
}

void Ship::stopTurningRight() {
	if(this->currentTurningSpeedDegreesPerSecond < 0) {
		this->currentTurningSpeedDegreesPerSecond = 0;
	}
}

void Ship::fire(std::string side) {
	if(this->isAlive()) {
		auto transform = this->getComponents()->getSingleByClass< Transform >();

		auto scene = this->getGameObjectSet().lock()->getOwner();
		auto ball = std::make_shared<Ball>();
		auto ballComponents = ball->getComponents();

		auto direction = (side == std::string("right") ? 1.0f : -1.0f) * transform->getRight();

		ballComponents->getSingleByClass< Transform >()->setPosition(
			transform->getPosition() + glm::vec3(0, 10, 0) + direction * 5.0f
		);

		ballComponents->add(std::make_shared< RigidBody >());
		ballComponents->add(std::make_shared< Projectile >());
		ballComponents->getSingleByClass< RigidBody >()->setVelocity(direction * 50.0f);
		scene->add(ball);
	}
}

void Ship::start() {
	this->water = this->getGameObjectSet().lock()->getSingleByClass<Water>() ;

	auto app = this->getApplication();
	auto fileName = app->getDataDirectory() + string("/models/galleon.obj");
	this->mesh = objMeshLoad(fileName.c_str());
}

void Ship::update(std::shared_ptr<FrameEventArgs> args) {
	BaseGameObject::update(args);

	auto gos = this->getGameObjectSet().lock();
	auto water = gos->getSingleByClass<Water>();
	auto terrain = gos->getSingleByClass<Terrain>();

	auto transform = this->getComponents()->getSingleByClass<Transform>();

	auto position = transform->getPosition();

	auto time = args->getTotalSeconds();

	auto heightThreshold = 3.0f;

	if(terrain->heightAtPositionAndTime(&position, time) - heightThreshold
		> water->heightAtPositionAndTime(&position, time)) {
		this->instantDeath();
	}
}

void Ship::fixedUpdate(std::shared_ptr<FrameEventArgs> args) {
	BaseGameObject::fixedUpdate(args);

	auto transform = this->getComponents()->getSingleByClass<Transform>();

	if(this->isAlive()) {
		float dt = args->getSeconds();

		// If acceleration has non-zero value, change velocity
		if(this->currentAcceleration != 0.0f) {
			auto dv = this->currentAcceleration * dt;
			this->velocity = glm::clamp(this->velocity + dv, Ship::MIN_VELOCITY, Ship::MAX_VELOCITY);
		}

		// If velocity has greater-than-zero value, move or rotate ship
		if(this->velocity > 0.0f) {
			// If angular speed is non-zero, rotate ship a little
			if(this->currentTurningSpeedDegreesPerSecond != 0) {
				auto dyaw = this->currentTurningSpeedDegreesPerSecond * dt;
				transform->rotateAroundLocal(glm::vec3(0, 1, 0), dyaw);
			}

			// Move ship using forward vector
			transform->translateLocal(glm::vec3(0, 0, -1) * this->velocity * dt);
		}
	}

	if(this->useWaterLevel) {
		auto water = this->gameObjectSet.lock()->getSingleByClass<Water>();

		// Modify ship y coordinate to match water height
		auto pos = transform->getPosition();
		float height = water->heightAtPositionAndTime(&pos, args->getTotalSeconds());

		pos.y = height;
		transform->setPosition(pos);
	}
}

void Ship::draw(std::shared_ptr<FrameEventArgs> args) {
	assert(this);

	auto transform = this->getComponents()->getSingleByClass<Transform>();
	auto position = transform->getPosition();

	// Model isn't facing -Z direction, we shall correct it
	glRotatef(90.0f, 0, 1, 0);

	// Models is too big so I need to scale it
	float scale = 0.3f;
	
	glScalef(scale, scale, scale);

	if(this->axes == Ship::Draw) {
		drawAxes(150.0f);
	}

	if(this->useWaterLevel) {
		// Calculate normal in ship position and roll/pitch according to this normal
		auto normal = this->water->normalAtPositionAndTime(&position, args->getTotalSeconds());

		auto vxy = glm::normalize(glm::vec3(normal.x, normal.y, 0));
		auto vzy = glm::normalize(glm::vec3(0, normal.y, normal.z));

		float rollRadians = std::asin(vxy.x);
		float pitchRadians = std::asin(vzy.z);

		glRotatef(glm::degrees(rollRadians), 0, 0, -1);
		glRotatef(glm::degrees(pitchRadians), 1, 0, 0);
	}


	if(this->axes == DrawWithRotation) {
		drawAxes(150.0f);
	}


	glColor3f(0.7f, 1.0f, 0.7f);

	
	// OBJMesh rendering
  	
	//TODO: refactor 	
	glBegin(GL_TRIANGLES);  	
	for (int i = 0; i < mesh->numIndices; ++i) {  	
		unsigned int index = mesh->indices[i];	  	
		float* vertex = (float*)((char*)mesh->vertices + index * mesh->stride);	
		float* normal = (float*)((char*)vertex + mesh->normalOffset);	
		if (mesh->hasNormals) {	
			glNormal3fv(normal);
		}  		
		glVertex3fv(vertex);	
	} 	
	glEnd();
}

void Ship::onCollide(std::shared_ptr< BaseGameObject > collider) {
	BaseGameObject::onCollide(collider);

	if(strcmp(collider->className(), "Ship") == 0) {
		this->instantDeath();
	}
}


Ship::~Ship() {
}

void Ship::setAxes(Ship::ShipDrawAxes axes) {
	this->axes = axes;
}

bool Ship::getUseWaterLevel() const {
	return this->useWaterLevel;
}

void Ship::setUseWaterLevel(bool value) {
	this->useWaterLevel = value;
}

int Ship::getHp() {
	return this->hp;
}

bool Ship::isDead() {
	return this->hp <= 0;
}

bool Ship::isAlive() {
	return this->hp > 0;
}

void Ship::damage(int damagePoints) {
	this->hp -= damagePoints;
}

void Ship::instantDeath() {
	this->hp = 0;
}
