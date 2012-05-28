#ifndef GAMEOBJECTSET_H
#define GAMEOBJECTSET_H

#include <list>
#include <boost/smart_ptr.hpp>

#include "objectset.h"

#include "basegameobject.h"

class Application;
class Scene;

class GameObjectSet : public ObjectSet<BaseGameObject, Scene> {
public: virtual const char* className() const {
		return "GameObjectSet";
	}

protected: virtual void onAdd(std::shared_ptr<BaseGameObject> object);
public: std::shared_ptr<Application> getApplication();
};

#endif