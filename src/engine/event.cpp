#include "precompile.h"
#include "event.h"

using namespace std;

Event::Event() : callbacks() {
	this->callbacks.push_back([] () { printf("event!"); } );
}


Event::~Event() {
}

void Event::raise() {
	for(auto i = this->callbacks.begin(); i != this->callbacks.end(); ++i) {
		auto callback = *i;
		callback();
	}
}