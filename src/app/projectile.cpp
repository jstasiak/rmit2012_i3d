#include "precompile.h"
#include "projectile.h"

#include "../engine/gameobject/basegameobject.h"
#include "../engine/gameobject/gameobjectset.h"
#include "../engine/gameobject/transform.h"

#include "water.h"

REGISTER(Projectile);

Projectile::Projectile() {
}

void Projectile::update(std::shared_ptr< FrameEventArgs > args) {
	// We want to destroy projectile once it gets under water

	auto go = this->getGameObject();
	auto gos = go->getGameObjectSet().lock();

	auto water = gos->getSingleByClass<Water>();
	
	auto transform = go->getComponents()->getSingleByClass<Transform>();
	auto position = transform->getPosition();
	auto waterpos = water->heightAtPositionAndTime(&position, args->getTotalSeconds());

	if(position.y < waterpos) {
		go->destroy();
		printf("destroying projectile\n");
	}
}

void Projectile::onCollide(std::shared_ptr< BaseGameObject > collider) {
	this->destroy();
}