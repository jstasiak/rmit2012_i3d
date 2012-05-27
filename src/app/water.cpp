#include "precompile.h"
#include "water.h"

#include "../engine/application.h"
#include "../engine/command.h"
#include "../engine/scene.h"
#include "../engine/texture.h"
#include "../engine/utils.h"
#include "../engine/gameobject/gameobjectset.h"
#include "../engine/gameobject/camera.h"
#include "../engine/gameobject/transform.h"

REGISTER(Water);

Water::Water()
	: 
	waveX((new Wave())->setAmplitude(3)->setLength(89)->setFrequency(0.7f)),
	waveZ((new Wave())->setAmplitude(3)->setLength(111)->setFrequency(0.5f)),
	waterTexture()
{
	

}


void Water::start() {
	auto app = this->getApplication();
	auto cs = app->getCommandSystem();

	auto textureFile = app->getDataDirectory() + std::string("/textures/tiles/water.jpg");
	this->waterTexture = std::shared_ptr< Texture >(new Texture(textureFile));
	
	cs->registerCommand("increase_water_tesselation", [this](command_parameters parameters) {
		this->doubleTesselationSafe();
	});

	cs->registerCommand("decrease_water_tesselation", [this](command_parameters parameters) {
		this->halveTesselationSafe();
	});

	cs->registerCommand("toggle_water_normals", [this](command_parameters parameters) {
		this->toggleNormals();
	});
}

void Water::setupMaterial() {
	Plane::setupMaterial();

	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	this->waterTexture->activate();
}


float Water::heightAtPositionAndTime(const glm::vec3* position, float time) const {
	// Sum two waves - one on XY and another on ZY plane
	return this->waveX->valueForPositionAndTime(position->x, time)
		+ this->waveZ->valueForPositionAndTime(position->z, time);
}

glm::vec3 Water::normalAtPositionAndTime(const glm::vec3* position, float time) const {
	assert(this);

	float dydx = this->waveX->derivativeForPositionAndTime(position->x, time);
	float dydz = this->waveZ->derivativeForPositionAndTime(position->z, time);

	// Calculate normals for both waves using method found on
	// http://goanna.cs.rmit.edu.au/~gl/teaching/i3d/2012/tutorial5.html
	auto vecx = glm::vec3(-dydx, 1, 0);
	auto vecz = glm::vec3(0, 1, -dydz);

	// Combine normal vectors
	auto result = glm::normalize(vecx + vecz);

	return result;
}


glm::vec2 Water::texcoordAtPositionAndTime(const glm::vec3& position, const float& time) const {
	return glm::vec2((position.x - time * 6.1f) / 110.0f,
		(position.z - time * 9.1f) / 140.0f);
}
