#include "utils.h"

#ifdef WIN32
	#include <Windows.h>
#endif

long long millisecondsNow() {
#ifdef WIN32
	return GetTickCount64();
#endif
}