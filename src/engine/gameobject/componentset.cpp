#include "precompile.h"

#include "componentset.h"

ComponentSet::ComponentSet() : gameObject()
{
}

ComponentSet::ComponentSet(boost::shared_ptr<BaseGameObject> gameObject)
	: gameObject(gameObject)
{
}

ComponentSet::iterator ComponentSet::begin() {
	return this->components.begin();
}

ComponentSet::iterator ComponentSet::end() {
	return this->components.end();
}