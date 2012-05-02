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

private: std::shared_ptr<BaseGameObject> gameObject;

private: int_components_t components;

public: ComponentSet();
public: ComponentSet(std::shared_ptr<BaseGameObject> gameObject);

public: template <class T> void add(T* component) {
	this->add(std::shared_ptr<T>(component));
}

public: template <class T> void add(std::shared_ptr<T> component) {
	this->components.push_back(component);
}

public: std::shared_ptr<BaseGameObject> getGameObject();

public: typedef int_components_t components_t;
public: components_t getList();
};

#endif // COMPONENTSET_H