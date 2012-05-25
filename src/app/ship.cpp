#include "precompile.h"
#include "ship.h"

#include "../engine/application.h"
#include "../engine/command.h"
#include "../engine/utils.h"
#include "water.h"

#include "../engine/gameobject/gameobjectset.h"
#include "../engine/gameobject/transform.h"

#include "../engine/gameobject/ball.h"
#include "../engine/gameobject/rigidbody.h"
#include "../engine/scene.h"

using namespace std;

REGISTER(Ship);

const float Ship::MIN_VELOCITY = 0.0f;
const float Ship::MAX_VELOCITY = 45.0f;
const float Ship::ACCELERATION = 30.0f;
const float Ship::TURNING_SPEED_DEGREES_PER_SECOND = 30.0f;

Ship::Ship()
	:
	currentAcceleration(0.0f),
	currentTurningSpeedDegreesPerSecond(0.0f),
	yaw(0),
	water(),
	mesh(0),
	useWaterLevel(true)
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
	auto transform = this->getComponents()->getSingleByClass< Transform >();

	auto scene = this->getGameObjectSet().lock()->getOwner();
	auto r = Registry::getSharedInstance();
	auto ball = r->create<Ball>();
	auto ballComponents = ball->getComponents();

	ballComponents->getSingleByClass< Transform >()->setPosition(transform->getPosition());
	ballComponents->add(r->create< RigidBody >());
	ballComponents->getSingleByClass< RigidBody >()->setVelocity(glm::vec3(0, 0, 50));
	scene->add(ball);
}

void Ship::start() {
	this->water = this->getGameObjectSet().lock()->getSingleByClass<Water>() ;

	auto app = this->getApplication();
	auto fileName = app->getDataDirectory() + string("/models/galleon.obj");
	this->mesh = objMeshLoad(fileName.c_str());
}

void Ship::update(std::shared_ptr<FrameEventArgs> args) {
	BaseGameObject::update(args);
}

void Ship::fixedUpdate(std::shared_ptr<FrameEventArgs> args) {
	BaseGameObject::fixedUpdate(args);

	auto transform = this->getComponents()->getSingleByClass<Transform>();

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
			this->yaw = std::fmod(this->yaw + dyaw, 360.0f);
		}

		// Calculate forward vector based on current yaw
		auto yawInRadians = glm::radians(this->yaw);
		auto unit = glm::vec3(
			std::cos(yawInRadians),
			0,
			-std::sin(yawInRadians));

		// Move ship using forward vector
		auto dx = unit * this->velocity * dt;
		transform->setPosition(transform->getPosition() + dx);
	}

	if(this->useWaterLevel) {
		auto water = this->gameObjectSet.lock()->getSingleByClass<Water>();

		// Modify ship y coordinate to match water height
		float height = water->heightAtPositionAndTime(&transform->getPosition(), args->getTotalSeconds());

		auto pos = transform->getPosition();
		pos.y = height;
		transform->setPosition(pos);
	}
}

void Ship::draw(std::shared_ptr<FrameEventArgs> args) {
	assert(this);

	auto transform = this->getComponents()->getSingleByClass<Transform>();
	auto position = transform->getPosition();

	// Models is too big so I need to scale it
	float scale = 0.3f;
	glScalef(scale, scale, scale);

	// Apply current yaw
	glRotatef(this->yaw, 0, 1, 0);

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