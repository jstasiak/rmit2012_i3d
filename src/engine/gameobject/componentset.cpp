#include "precompile.h"

#include "componentset.h"
#include "basegameobject.h"

void ComponentSet::onAdd(std::shared_ptr<BaseComponent> object) {
	object->setGameObject(this->owner.lock()->getSharedPointer<BaseGameObject>());
}