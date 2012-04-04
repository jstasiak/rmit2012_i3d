#include "precompile.h"
#include "utils.h"

long long millisecondsNow() {
	long long milliseconds = 0;
	auto time = boost::posix_time::microsec_clock::local_time();
	auto d = boost::posix_time::time_duration(time.time_of_day());
	milliseconds = d.total_milliseconds();
	return milliseconds;
}