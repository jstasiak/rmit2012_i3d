#include "precompile.h"

#include "basegameobject.h"
#include "gameobjectset.h"

using namespace std;

REGISTER(BaseGameObject);

BaseGameObject::BaseGameObject()
	: gameObjectSet(), components(0)
{
}

std::shared_ptr<ComponentSet> BaseGameObject::getComponents() {
	if(!this->components) {
		this->components = make_shared<ComponentSet>();
		this->components->setOwner(this->getSharedPointer<BaseGameObject>());
		this->components->add(Registry::getSharedInstance()->create<BaseComponent>("Transform"));
	}

	return this->components;
}


void BaseGameObject::start() {
	auto l = this->getComponents()->getList();

	for(auto i = l.begin(); i != l.end(); ++i) {
		auto component = *i;
		component->start();
	}
}

std::shared_ptr<Application> BaseGameObject::getApplication() {
	return this->gameObjectSet.lock()->getApplication();
}