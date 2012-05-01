#ifndef OBJECT_H
#define OBJECT_H

#define CLASS_NAME( _name) \
	public: virtual const char* GetClassName() const { return " ## "; };

class Object {
public: virtual ~Object() {}

CLASS_NAME(Object);
};

#endif