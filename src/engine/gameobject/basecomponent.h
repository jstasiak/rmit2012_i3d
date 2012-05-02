#ifndef BASECOMPONENT_H
#define BASECOMPONENT_H

#include <boost/smart_ptr.hpp>

#include "../object.h"
class BaseGameObject;
class FrameEventArgs;

class BaseComponent : Object {
CLASS_NAME(BaseComponent);

private: std::shared_ptr<BaseGameObject> gameObject;

public: BaseComponent() : gameObject() {}

public: std::shared_ptr<BaseGameObject> GetGameObject() {
		return this->gameObject;
	}

public: void SetGameObject(std::shared_ptr<BaseGameObject> gameObject) {
		this->gameObject = gameObject;
	}

public: virtual void start() {}
public: virtual void update(std::shared_ptr<FrameEventArgs> args) {}

public: ~BaseComponent() {}

};

#endif // BASECOMPONENT_H