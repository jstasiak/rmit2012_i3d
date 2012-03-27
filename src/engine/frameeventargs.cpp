#include "frameeventargs.h"

const EventArgs* const EventArgs::empty = new EventArgs();

float FrameEventArgs::getSeconds() const
{
    return this->seconds;
}

FrameEventArgs* FrameEventArgs::createFromSeconds( float seconds )
{
    return new FrameEventArgs(seconds);
}

FrameEventArgs::FrameEventArgs( float seconds ) : seconds(seconds)
{

}
