#ifndef GAMEOBJECTSET_H
#define GAMEOBJECTSET_H

#include <list>
#include <boost/smart_ptr.hpp>

#include "../object.h"

class BaseGameObject;
class Application;

class GameObjectSet : public Object {
	Q_OBJECT

private: std::weak_ptr<Application> application;

private: typedef std::list < std::shared_ptr <BaseGameObject> > IntGameObjects;

private: IntGameObjects gameObjects;

public: GameObjectSet();

public: template <class T> void add(std::shared_ptr<T> gameObject) {
		gameObject->setGameObjectSet(this->getSharedPointer<GameObjectSet>());
		this->gameObjects.push_back(gameObject);
	}

public: typedef IntGameObjects GameObjects;
public: GameObjects getList();

public: template<class T> std::shared_ptr<T> getSingleByClass() {
		auto multiple = this->getMultipleByClass<T>();
		auto size = multiple.size();
		if(size == 0) {
			throw std::exception("No objects of that class");
		}
		else if(size > 1) {
			throw std::exception("More than one object of that class");
		}

		return multiple.front();
	}

public: template<class T> std::list <std::shared_ptr <T> > getMultipleByClass() {
		auto name = T::staticMetaObject.className();
		std::list <std::shared_ptr <T> > objects;
		for(auto i = this->gameObjects.begin(); i != this->gameObjects.end(); ++i) {
			auto o = *i;
			if(strcmp(o->metaObject()->className(), name) == 0) {
				objects.push_back(o->getSharedPointer<T>());
			}
		}
		return objects;
	}

public: std::weak_ptr<Application> getApplication();
public: void setApplication(std::weak_ptr<Application> value);
};

#endif