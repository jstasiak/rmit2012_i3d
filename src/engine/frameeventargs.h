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
    FrameEventArgs(float seconds);
public:
    float getSeconds() const;
    static FrameEventArgs* createFromSeconds(float seconds);
};

#endif //FRAMEEVENTARGS_H