#include "app/myapp.h"

#include <cstdio>
#include <exception>
using namespace std;

int main()
{
	int returnCode = 0;
	Application* app = 0;
	try {
		app = new MyApp();
		int returnCode = app->run();
	}
	catch(exception* e) {
		printf("Exception occured: %s\n\n", e->what());
		returnCode = -1;
	}
	
	if(app) {
		delete app;
	}

	return returnCode;
}