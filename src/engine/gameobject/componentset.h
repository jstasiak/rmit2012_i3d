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
public: typedef std::list < boost::shared_ptr <BaseComponent> > components_t;
public: typedef components_t::iterator iterator;

private: boost::shared_ptr<BaseGameObject> gameObject;

private: components_t components;

public: ComponentSet();
public: ComponentSet(boost::shared_ptr<BaseGameObject> gameObject);

public: template <class T> void add(T* component) {
	this->add(boost::shared_ptr<T>(component));
}

public: template <class T> void add(boost::shared_ptr<T> component) {
	this->components.push_back(component);
}

public: components_t::iterator begin();
public: components_t::iterator end();

public: boost::shared_ptr<BaseGameObject> getGameObject();
public: void setGameObject(boost::shared_ptr<BaseGameObject> gameObject);
};

#endif // COMPONENTSET_H