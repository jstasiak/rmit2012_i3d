#include "precompile.h"
#include "app/myapp.h"

using namespace std;

int main(int argc, char** argv) {
	int returnCode = 0;
	try {
		auto application = make_shared<MyApp>();
		application->applyCommandlineParameters(argc, argv);
		returnCode = application->run();
	}
	catch(const exception& e) {
		printf("Exception occured: %s\n\n", e.what());
		returnCode = -1;
	}

	return returnCode;
}