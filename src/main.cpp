#include "precompile.h"
#include "app/myapp.h"

using namespace std;

int main(int argc, char** argv) {
	int returnCode = 0;
	try {
		boost::scoped_ptr<Application> application(new MyApp());
		returnCode = application->run();
	}
	catch(exception* e) {
		printf("Exception occured: %s\n\n", e->what());
		returnCode = -1;
	}

	return returnCode;
}