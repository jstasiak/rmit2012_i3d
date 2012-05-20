#ifndef OBJECT_H
#define OBJECT_H

#include <exception>

#include <boost/smart_ptr.hpp>
#include <QObject>
#include <QMap>
#include <QMetaType>
#include <QString>

class Object : public QObject, public std::enable_shared_from_this<Object> {
Q_OBJECT

private: std::string name;

public: Object() : name("")
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
};

class Registry {
private: QMap<QString, const QMetaObject*> metaLookup;

public: Registry() : metaLookup() {

	}

public: void registerMetaObject(const QMetaObject* metaObject) {
		this->metaLookup.insert(metaObject->className(), metaObject);
	}	

public: static Registry* getSharedInstance() {
		static Registry* instance = new Registry();
		return instance;
	}

public: template<class T> std::shared_ptr<T> create() {
		return this->create<T>(T::staticMetaObject.className());
	}

public: template<class T> std::shared_ptr<T> create(const QString& className) {
		auto mo = this->getByName(className);
		if(!mo) {
			throw std::exception("No such class registered");
		}

		QObject* qobjptr = mo->newInstance();
		T* tptr = qobject_cast<T*>(qobjptr);

		printf("Got object of class %s, casting to %s\n", qobjptr->metaObject()->className(),
			T::staticMetaObject.className());	

		if(!tptr) {
			delete qobjptr;
			throw std::exception("Class mismatch");
		}
		
		return std::shared_ptr<T>(tptr);
	}

private: const QMetaObject* getByName(const QString& name) {
		return this->metaLookup.value(name);
	}	
};


template <class T> class Registrator {
public: Registrator() {
			Registry::getSharedInstance()->registerMetaObject(&T::staticMetaObject);
		}
};

#define REGISTER(className) Registrator<className> registratorFor##className;

#endif