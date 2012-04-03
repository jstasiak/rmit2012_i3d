#include "utils.h"

#ifdef WIN32
#include <Windows.h>
#endif

long long millisecondsNow() {
	long long milliseconds = 0;
#ifdef WIN32
	milliseconds = GetTickCount64();
#endif
	return milliseconds;
}