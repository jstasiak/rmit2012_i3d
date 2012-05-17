#include "precompile.h"

#include "basegameobject.h"
#include "gameobjectset.h"

using namespace std;

REGISTER(BaseGameObject);

BaseGameObject::BaseGameObject()
	: gameObjectSet(), components(0)
{
}

void BaseGameObject::initialize() {
	if(!this->components) {
		this->setComponents(make_shared<ComponentSet>());
	}
}


void BaseGameObject::start() {
	auto l = this->components->getList();

	for(auto i = l.begin(); i != l.end(); ++i) {
		auto component = *i;
		component->start();
	}
}

shared_ptr<Application> BaseGameObject::getApplication() {
	return this->gameObjectSet.lock()->getOwner();
}

void BaseGameObject::setComponents(std::shared_ptr< ComponentSet > value) {
	if(this->components) {
		this->components->setOwner(std::shared_ptr< BaseGameObject >(0));
	}

	this->components = value;
	value->setOwner(this->getSharedPointer<BaseGameObject>());
}