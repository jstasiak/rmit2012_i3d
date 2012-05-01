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
	auto begin = this->components.begin();
	auto end = this->components.end();

	auto sa = boost::shared_ptr<FrameEventArgs>(args);
	for(auto i = begin; i != end; ++i) {
		auto component = *i;
		component->update(sa);
	}
}

public: void draw(boost::shared_ptr<FrameEventArgs> args) {}

public: ComponentSet& GetComponents() {
		return this->components;
	}

};

#endif