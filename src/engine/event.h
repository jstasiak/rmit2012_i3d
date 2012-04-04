#ifndef EVENT_H
#define EVENT_H

#include <list>
#include <functional>
using namespace std;

typedef function<void ()> EventCallback;

class Event
{
	list<EventCallback> callbacks;

public:
	Event(void);
	virtual ~Event(void);

	void raise();
};

#endif //EVENT_H
