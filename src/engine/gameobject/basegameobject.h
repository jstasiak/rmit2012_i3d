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

private: std::shared_ptr<ComponentSet> components;

private: bool active;

public: Q_INVOKABLE BaseGameObject();

public: virtual void start();

public: virtual void fixedUpdate(std::shared_ptr<FrameEventArgs> args) {
		this->fixedUpdateComponents(args);
	}

private: void fixedUpdateComponents(std::shared_ptr<FrameEventArgs> args) {
		auto l = this->getComponents()->getList();

		for(auto i = l.begin(); i != l.end(); ++i) {
			auto component = *i;
			component->fixedUpdate(args);
		}
	}

public: virtual void update(std::shared_ptr<FrameEventArgs> args) {
		this->updateComponents(args);
	}

private: void updateComponents(std::shared_ptr<FrameEventArgs> args) {
		auto l = this->getComponents()->getList();

		for(auto i = l.begin(); i != l.end(); ++i) {
			auto component = *i;
			component->update(args);
		}
	}

public: virtual void onCollide(std::shared_ptr< BaseGameObject > collider);

public: virtual void draw(std::shared_ptr<FrameEventArgs> args) {}



public: std::shared_ptr<ComponentSet> getComponents();

public: std::weak_ptr<GameObjectSet> getGameObjectSet() {
		return this->gameObjectSet;
	}

public: void setGameObjectSet(std::shared_ptr<GameObjectSet> value) {
		this->gameObjectSet = value;
	}

public: std::shared_ptr<Application> getApplication();

public: virtual void destroyImmediately();

public: bool isActive() const;
public: void setActive(bool value);
public: void activate();
public: void deactivate();

};

#endif