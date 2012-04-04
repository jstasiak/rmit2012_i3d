#ifndef FRAMEEVENTARGS_H
#define FRAMEEVENTARGS_H

class EventArgs {
private:
	static const EventArgs* const empty;
public:
	virtual ~EventArgs() {}
	static const EventArgs* const getEmpty() {
		return EventArgs::empty;
	}
};

class FrameEventArgs : public EventArgs {
private:
	float seconds;
	float totalSeconds;
	FrameEventArgs(float seconds, float totalSeconds);
public:
	float getSeconds() const;
	float getTotalSeconds() const;
	static FrameEventArgs* createFromSecondsAndTotalSeconds(float seconds, float totalSeconds);
};

#endif //FRAMEEVENTARGS_H