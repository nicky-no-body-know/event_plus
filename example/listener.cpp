#include "listener.h"
#include "event.h"
#include "event_base.h"
#include "socket.h"

NAMESPACE_BEGIN
int ServerListener::callback(int fd, int res) const
{
	int newfd;
	struct sockaddr_in addr;
	socklen_t addrlen;
	
	_base.get_log().LOG_P(LOG_INFO, "fd = %d, res = %d", fd, res);
	if (EV_READ & res)
	{
		newfd = EventUtil_accept(fd, (struct sockaddr*)&addr, &addrlen);
		Event *newEvent = new Event(newfd, _cb, EV_READ | EV_WRITE);
		_base.add_event(*newEvent);	
	}
	return 0;
}

NAMESPACE_END
