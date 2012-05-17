#ifndef GAMEOBJECTSET_H
#define GAMEOBJECTSET_H

#include <list>
#include <boost/smart_ptr.hpp>

#include "objectset.h"

#include "basegameobject.h"

class Application;

class GameObjectSet : public ObjectSet<BaseGameObject, Application> {
	Q_OBJECT

protected: virtual void onAdd(std::shared_ptr<BaseGameObject> object);
};

#endif