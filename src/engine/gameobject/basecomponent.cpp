#include "precompile.h"

#include "basecomponent.h"

void BaseComponent::destroyImmediately() {
	this->gameObject.reset();
}