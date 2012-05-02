#ifndef BASEGAMEOBJECT_H
#define BASEGAMEOBJECT_H

#include "../frameeventargs.h"
#include "componentset.h"
#include "basecomponent.h"

class BaseGameObject {
protected: ComponentSet components;

public: BaseGameObject() : components() {}

public: void update(boost::shared_ptr<FrameEventArgs> args) {
	this->updateComponents(args);
}

private: void updateComponents(boost::shared_ptr<FrameEventArgs> args) {
	auto l = this->components.getList();

	for(auto i = l.begin(); i != l.end(); ++i) {
		auto component = *i;
		component->update(args);
	}
}

public: void draw(boost::shared_ptr<FrameEventArgs> args) {}

public: ComponentSet& GetComponents() {
		return this->components;
	}

};

#endif