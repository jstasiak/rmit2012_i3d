#include "frameeventargs.h"

const EventArgs* const EventArgs::empty = new EventArgs();

float FrameEventArgs::getSeconds() const {
	return this->seconds;
}

float FrameEventArgs::getTotalSeconds() const {
	return this->totalSeconds;
}

FrameEventArgs* FrameEventArgs::createFromSecondsAndTotalSeconds( float seconds, float totalSeconds ) {
	return new FrameEventArgs(seconds, totalSeconds);
}

FrameEventArgs::FrameEventArgs( float seconds, float totalSeconds )
	: seconds(seconds), totalSeconds(totalSeconds) {

}
