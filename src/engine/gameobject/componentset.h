#ifndef COMPONENTSET_H
#define COMPONENTSET_H

#include <list>
#include <boost/smart_ptr.hpp>

#include "../object.h"
#include "objectset.h"
#include "basecomponent.h"


class BaseGameObject;

class ComponentSet : public ObjectSet<BaseComponent, BaseGameObject> {
	Q_OBJECT

protected: virtual void onAdd(std::shared_ptr<BaseComponent> object);
};

#endif