#ifndef OBJECT_H
#define OBJECT_H

#include <exception>

#include <boost/smart_ptr.hpp>


class NotImplemented : public std::exception {
public: virtual const char* what() const {
		return "NotImplemented";
	}
};

class Object : public std::enable_shared_from_this<Object> {

private: std::string name;
private: bool destroying;

public: virtual const char* className() const {
		return "Object";
	}

public: Object() : name(""), destroying(false)
	{
	}

public: virtual ~Object() {}

public: template <class T> std::shared_ptr<T> getSharedPointer() {
		return std::dynamic_pointer_cast<T>(this->shared_from_this());
	}

public: std::string getName() const {
		return this->name;
	}

public: void setName(std::string value) {
		this->name = value;
	}


public: void destroy() {
		this->destroying = true;
	}

public: bool isDestroying() const {
		return this->destroying;
	}

public: virtual void destroyImmediately() {
		throw NotImplemented();
	}
};



#endif