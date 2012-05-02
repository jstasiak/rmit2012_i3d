#ifndef COMPONENTSET_H
#define COMPONENTSET_H

#include <list>
#include <boost/smart_ptr.hpp>

class BaseComponent;
class BaseGameObject;

struct string_comparator {
	bool operator()(char const *a, char const *b) {
		return std::strcmp(a, b) < 0;
	}
};

class ComponentSet {
private: typedef std::list < std::shared_ptr <BaseComponent> > int_components_t;

private: BaseGameObject* gameObject;

private: int_components_t components;

public: ComponentSet(BaseGameObject* gameObject);

public: template <class T> void add(std::shared_ptr<T> component) {
	component->setGameObject(this->gameObject->getSharedPointer<BaseGameObject>());
	this->components.push_back(component);
}

public: std::shared_ptr<BaseGameObject> getGameObject();

public: typedef int_components_t components_t;
public: components_t getList();
};

#endif // COMPONENTSET_H