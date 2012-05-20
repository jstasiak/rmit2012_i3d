#include "precompile.h"

#include "gameobjectset.h"
#include "basegameobject.h"
#include "../scene.h"

void GameObjectSet::onAdd(std::shared_ptr<BaseGameObject> object) {
	auto go = std::dynamic_pointer_cast<BaseGameObject>(object);
	go->setGameObjectSet(this->getSharedPointer< GameObjectSet >());
}

std::shared_ptr<Application> GameObjectSet::getApplication() {
	return this->getOwner()->getApplication();
}