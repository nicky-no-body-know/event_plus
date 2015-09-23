#ifndef EVENT_PLUS_SOCKET_H
#define EVENT_PLUS_SOCKET_H

#include "comm.h"
#include "event_base.h"
#include "event_util.h"
#include "log.h"

NAMESPACE_BEGIN
class SocketBase
{
public:
	SocketBase(EventBase& base) : _base(base) {}
private:
	SocketBase(SocketBase&);
	SocketBase& operator=(SocketBase&);

protected:
	EventBase& _base;
};


class TcpSocket : public SocketBase
{
public:
	TcpSocket(EventBase &base) : SocketBase(base)
	{
		_fd = EventUtil_socket(AF_INET, SOCK_STREAM, 0);
		Log& log = _base.get_log();
		log.log_p(LOG_INFO, __FILE__, __FUNCTION__, __LINE__, "fd = %d", _fd);
	}
	explicit TcpSocket(EventBase& base, int fd) : SocketBase(base) { _fd = fd; }

	TcpSocket(EventBase& base, const TcpSocket& sock) : SocketBase(base) { _fd = sock.get_file_desc(); }
	TcpSocket& operator=(const TcpSocket& sock) { _fd = sock.get_file_desc(); return *this; }

	int get_file_desc() const { return _fd; }
private:
	int _fd;
};
NAMESPACE_END
#endif
