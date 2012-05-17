#include "precompile.h"

#include "gameobjectset.h"
#include "basegameobject.h"

void GameObjectSet::onAdd(std::shared_ptr<BaseGameObject> object) {
	auto go = std::dynamic_pointer_cast<BaseGameObject>(object);
	go->setGameObjectSet(this->getSharedPointer< GameObjectSet >());
}