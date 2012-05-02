#include "precompile.h"

#include "componentset.h"

ComponentSet::ComponentSet() : gameObject()
{
}

ComponentSet::ComponentSet(boost::shared_ptr<BaseGameObject> gameObject)
	: gameObject(gameObject)
{
}

boost::shared_ptr<BaseGameObject> ComponentSet::getGameObject() {
	return this->gameObject;
}

ComponentSet::components_t ComponentSet::getList() {
	return this->components;
}