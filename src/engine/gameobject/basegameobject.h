#ifndef BASEGAMEOBJECT_H
#define BASEGAMEOBJECT_H

#include "../frameeventargs.h"
#include "basecomponent.h"
#include "componentset.h"
#include "../object.h"

class BaseGameObject : public Object {
	Q_OBJECT

protected: std::shared_ptr<ComponentSet> components;

public: BaseGameObject() : components(new ComponentSet(this)) {}

public: virtual void update(std::shared_ptr<FrameEventArgs> args) {
	this->updateComponents(args);
}

private: void updateComponents(std::shared_ptr<FrameEventArgs> args) {
	auto l = this->components->getList();

	for(auto i = l.begin(); i != l.end(); ++i) {
		auto component = *i;
		component->update(args);
	}
}

public: virtual void draw(std::shared_ptr<FrameEventArgs> args) {}

public: std::shared_ptr<ComponentSet> getComponents() {
		return this->components;
	}

};

#endif