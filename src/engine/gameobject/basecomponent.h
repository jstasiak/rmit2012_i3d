#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <boost/smart_ptr.hpp>

#include "../object.h"

class BaseGameObject;
class FrameEventArgs;

class BaseComponent : public Object {
	Q_OBJECT

protected: std::shared_ptr<BaseGameObject> gameObject;

public: BaseComponent() : gameObject() {}

public: std::shared_ptr<BaseGameObject> getGameObject() {
		return this->gameObject;
	}

public: void setGameObject(std::shared_ptr<BaseGameObject> gameObject) {
		this->gameObject = gameObject;
	}

public: virtual void start() {}
public: virtual void update(std::shared_ptr<FrameEventArgs> args) {}

public: ~BaseComponent() {}

public: virtual void destroyImmediately();

};

#endif