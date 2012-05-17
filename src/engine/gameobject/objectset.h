#ifndef OBJECTSET_H
#define OBJECTSET_H

#include <list>
#include <exception>
#include <boost/smart_ptr.hpp>

#include "../object.h"


class ObjectSetException : public std::exception {};
class DoesNotExist : public ObjectSetException {};
class MultipleObjectsReturned : public ObjectSetException {};

template< class TElement, class TOwner >
class ObjectSet : public Object {

protected: std::weak_ptr<TOwner> owner;

private: std::list< std::shared_ptr< TElement > > objects;


public: ObjectSet() : owner(), objects()
	{
	}

public: void add(std::shared_ptr<TElement> object) {
		this->add<TElement>(object);
	}

public: template< class T > void add(std::shared_ptr<T> object) {
		auto casted = object->getSharedPointer<TElement>();
		this->objects.push_back(casted);
		this->onAdd(casted);
	}

protected: virtual void onAdd(std::shared_ptr<TElement> object) {}

public: std::list< std::shared_ptr< TElement > > getList() {
		return this->objects;
	}

public: template<class T> std::shared_ptr<T> getSingleByClass() {
		auto multiple = this->getMultipleByClass<T>();
		auto size = multiple.size();

		if(size == 0) {
			throw DoesNotExist();
		}
		else if(size > 1) {
			throw MultipleObjectsReturned();
		}

		return multiple.front();
	}

public: template<class T> std::list< std::shared_ptr< T > > getMultipleByClass() {
		auto name = T::staticMetaObject.className();
		std::list< std::shared_ptr< T > > objects;

		for(auto i = this->objects.begin(); i != this->objects.end(); ++i) {
			auto o = *i;
			if(strcmp(o->metaObject()->className(), name) == 0) {
				objects.push_back(o->getSharedPointer<T>());
			}
		}
		return objects;
	}

public: std::shared_ptr<TOwner> getOwner() {
		return this->owner.lock();
	}

public: void setOwner(std::weak_ptr<TOwner> value) {
		this->owner = value;
	}
};

#endif