#include "precompile.h"

#include "basecomponent.h"

REGISTER(BaseComponent);

void BaseComponent::destroyImmediately() {
	this->gameObject.reset();
}