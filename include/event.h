#ifndef EVENT_PLUS_EVENT_H
#define EVENT_PLUS_EVENT_H

#define EV_READ 0x1
#define EV_WRITE 0x2

#include "comm.h"

NAMESPACE_BEGIN

struct EventCallback
{
	virtual int callback(int fd, int res) const = 0;
};

class Event
{
public:
	Event(int fd, EventCallback *callback, int flags);
	//Event& operator=(const Event&);
	~Event(){}

	int get_file_desc() { return _fd; }

	int get_flags() const { return _flags; }
	void set_flags(int flags) { _flags = flags; }
	
	int get_ret_flags() const { return _retFlags; }
	void set_ret_flags(int retFlags) { _retFlags = retFlags; }

	void callback()
	{
		_callback->callback(_fd, _retFlags);
	}
private:
	int _fd;
	EventCallback *_callback;
	int _flags;
	int _retFlags;
};

NAMESPACE_END
#endif
