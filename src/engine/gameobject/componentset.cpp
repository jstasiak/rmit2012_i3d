#include "precompile.h"

#include "componentset.h"
#include "basegameobject.h"

ComponentSet::ComponentSet(BaseGameObject* gameObject)
	: gameObject(gameObject)
{
}

std::shared_ptr<BaseGameObject> ComponentSet::getGameObject() {
	return this->gameObject->getSharedPointer<BaseGameObject>();
}

ComponentSet::components_t ComponentSet::getList() {
	return this->components;
}