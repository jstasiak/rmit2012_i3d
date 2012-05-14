#ifndef BASEGAMEOBJECT_H
#define BASEGAMEOBJECT_H

#include "../frameeventargs.h"
#include "basecomponent.h"
#include "../object.h"
#include "componentset.h"
#include "gameobjectset.h"


class BaseGameObject : public Object {
	Q_OBJECT
protected: std::weak_ptr<GameObjectSet> gameObjectSet;
protected: std::shared_ptr<ComponentSet> components;

public: Q_INVOKABLE BaseGameObject() : gameObjectSet(), components(new ComponentSet(this)) {}

public: virtual void start() {
		auto l = this->components->getList();

		for(auto i = l.begin(); i != l.end(); ++i) {
			auto component = *i;
			component->start();
		}
	}

public: virtual void update(std::shared_ptr<FrameEventArgs> args) {
		this->updateComponents(args);
	}

private: void updateComponents(std::shared_ptr<FrameEventArgs> args) {
		auto l = this->components->getList();

		for(auto i = l.begin(); i != l.end(); ++i) {
			auto component = *i;
			component->update(args);
		}
	}

public: virtual void draw(std::shared_ptr<FrameEventArgs> args) {}

public: std::shared_ptr<ComponentSet> getComponents() {
		return this->components;
	}

public: std::weak_ptr<GameObjectSet> getGameObjectSet() {
		return this->gameObjectSet;
	}

public: void setGameObjectSet(std::weak_ptr<GameObjectSet> value) {
		this->gameObjectSet = value;
	}

protected: std::weak_ptr<Application> getApplication() {
		return this->gameObjectSet.lock()->getApplication();
	}
};

#endif