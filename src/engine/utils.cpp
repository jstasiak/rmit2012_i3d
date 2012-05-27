#include "precompile.h"
#include "utils.h"

long long millisecondsNow() {
	long long milliseconds = 0;
	auto time = boost::posix_time::microsec_clock::local_time();
	auto d = boost::posix_time::time_duration(time.time_of_day());
	milliseconds = d.total_milliseconds();
	return milliseconds;
}

void drawAxes(float length) {
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3f(1, 0, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(length, 0, 0);

		glColor3f(0, 1, 0);

		glVertex3f(0, 0, 0);
		glVertex3f(0, length, 0);

		glColor3f(0, 0, 1);

		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, length);
	glEnd();
	glEnable(GL_LIGHTING);
}
