#ifndef BASEGAMEOBJECT_H
#define BASEGAMEOBJECT_H

#include "../frameeventargs.h"
#include "basecomponent.h"
#include "../object.h"
#include "componentset.h"

class GameObjectSet;
class Application;

class BaseGameObject : public Object {
	Q_OBJECT
protected: std::weak_ptr<GameObjectSet> gameObjectSet;
protected: std::shared_ptr<ComponentSet> components;

public: Q_INVOKABLE BaseGameObject();

public: void initialize();

public: virtual void start();

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
		assert(this->components);
		return this->components;
	}

public: std::weak_ptr<GameObjectSet> getGameObjectSet() {
		return this->gameObjectSet;
	}

public: void setGameObjectSet(std::shared_ptr<GameObjectSet> value) {
		this->gameObjectSet = value;
	}

public: std::shared_ptr<Application> getApplication();

private: void setComponents(std::shared_ptr<ComponentSet> value);

};

#endif