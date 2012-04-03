#include "myapp.h"

int main()
{
	Application* app = new MyApp();
	int returnCode = app->run();
	delete app;
	return returnCode;
}