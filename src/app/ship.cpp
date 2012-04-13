#include "precompile.h"
#include "ship.h"

const float Ship::MIN_VELOCITY = 0.0f;
const float Ship::MAX_VELOCITY = 45.0f;
const float Ship::ACCELERATION = 30.0f;
const float Ship::TURNING_SPEED_DEGREES_PER_SECOND = 30.0f;

Ship::Ship()
	: position(0, 0, 0),
	currentAcceleration(0),
	currentTurningSpeedDegreesPerSecond(0),
	yaw(0)
{
	this->mesh = objMeshLoad("models/galleon.obj");
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


void Ship::update(FrameEventArgs* args) {
	float dt = args->getSeconds();

	// If acceleration has non-zero value, change velocity
	if(this->currentAcceleration != 0) {
		auto dv = this->currentAcceleration * dt;
		this->velocity = glm::clamp(this->velocity + dv, Ship::MIN_VELOCITY, Ship::MAX_VELOCITY);
	}

	// If velocity has greater-than-zero value, move or rotate ship
	if(this->velocity > 0) {
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
		this->position += dx;
	}


}

void Ship::draw(FrameEventArgs* args) {
	glPushMatrix();
	glTranslatef(this->position.x, this->position.y, this->position.z);

	float scale = 0.4f;
	glScalef(scale, scale, scale);

	glRotatef(this->yaw, 0, 1, 0);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < mesh->numIndices; ++i) {
		unsigned int index = mesh->indices[i];
		float* vertex = (float*)((char*)mesh->vertices + index * mesh->stride);
		float* normal = (float*)((char*)vertex + mesh->normalOffset);
		if (mesh->hasNormals)
			glNormal3fv(normal);

		glVertex3fv(vertex);
	}
	glEnd();

	glPopMatrix();
}

Ship::~Ship() {
	if(this->mesh) {
		objMeshFree(&this->mesh);
		this->mesh = 0;
	}
}

glm::vec3* Ship::getPosition() {
	return &(this->position);
}

Ship* Ship::setPosition(const glm::vec3* value) {
	this->position = *value;
	return this;
}