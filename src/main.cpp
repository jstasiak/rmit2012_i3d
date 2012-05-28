#include "precompile.h"
#include "engine/application.h"

using namespace std;

int main(int argc, char** argv) {
	int returnCode = 0;
	try {
		auto application = make_shared<Application>();
		returnCode = application->runWithCommandLineParameters(argc, argv);
	}
	catch(const exception& e) {
		printf("Exception occured: %s\n\n", e.what());
		returnCode = -1;
	}

	return returnCode;
}