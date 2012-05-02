#ifndef OBJECT_H
#define OBJECT_H

#include <boost/smart_ptr.hpp>

#define CLASS_NAME( _name) \
	public: virtual const char* GetClassName() const { return " ## "; };

class Object : public std::enable_shared_from_this<Object> {
CLASS_NAME(Object);

public: virtual ~Object() {}

public: template <class T> std::shared_ptr<T> getSharedPointer() {
		return std::dynamic_pointer_cast<T>(this->shared_from_this());
	}
};

#endif