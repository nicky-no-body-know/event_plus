#include "event.h"
NAMESPACE_BEGIN

Event::Event(int fd, EventCallback *callback, int flags) :
	_fd(fd), _callback(callback), _flags(flags)
{
}

NAMESPACE_END
