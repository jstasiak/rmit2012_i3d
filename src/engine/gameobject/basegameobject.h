#ifndef BASEGAMEOBJECT_H
#define BASEGAMEOBJECT_H

#include "../frameeventargs.h"
#include "componentset.h"
#include "basecomponent.h"

class BaseGameObject {
protected: ComponentSet components;

public: BaseGameObject() : components() {}

public: void update(std::shared_ptr<FrameEventArgs> args) {
	this->updateComponents(args);
}

private: void updateComponents(std::shared_ptr<FrameEventArgs> args) {
	auto l = this->components.getList();

	for(auto i = l.begin(); i != l.end(); ++i) {
		auto component = *i;
		component->update(args);
	}
}

public: void draw(std::shared_ptr<FrameEventArgs> args) {}

public: ComponentSet& GetComponents() {
		return this->components;
	}

};

#endif