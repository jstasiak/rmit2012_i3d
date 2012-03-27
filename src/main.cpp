#include "engine/application.h"

int main()
{
	auto app = new Application();
	int returnCode = app->run();
	delete app;
	return returnCode;
}