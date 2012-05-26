#include "precompile.h"

#include "basegameobject.h"
#include "gameobjectset.h"
#include "basecomponent.h"

using namespace std;

REGISTER(BaseGameObject);

BaseGameObject::BaseGameObject()
	:
	gameObjectSet(),
	components(0),
	active(true)
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


void BaseGameObject::onCollide(std::shared_ptr< BaseGameObject > collider) {
	auto l = this->getComponents()->getList();

	for(auto i = l.begin(); i != l.end(); ++i) {
		auto component = *i;
		component->onCollide(collider);
	}
}


std::shared_ptr<Application> BaseGameObject::getApplication() {
	return this->gameObjectSet.lock()->getApplication();
}

void BaseGameObject::destroyImmediately() {
	this->getComponents()->destroyImmediately();
	this->gameObjectSet.reset();
}


bool BaseGameObject::isActive() const {
	return this->active;
}

void BaseGameObject::setActive(bool value) {
	this->active = value;
}

void BaseGameObject::activate() {
	this->setActive(true);
}

void BaseGameObject::deactivate() {
	this->setActive(false);
}