#include "precompile.h"

#include "gameobjectset.h"

GameObjectSet::GameObjectSet() : gameObjects() {

}

GameObjectSet::GameObjects GameObjectSet::getList() {
	return this->gameObjects;
}

std::weak_ptr<Application> GameObjectSet::getApplication() {
	return this->application;
}

void GameObjectSet::setApplication(std::weak_ptr<Application> value) {
	this->application = value;
}