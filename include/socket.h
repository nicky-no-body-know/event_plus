#ifndef EVENT_PLUS_SOCKET_H
#define EVENT_PLUS_SOCKET_H

#include "comm.h"
#include "event_util.h"

NAMESPACE_BEGIN
class SocketBase
{
public:
	SocketBase() {}
private:
	SocketBase(SocketBase&);
	SocketBase& operator=(SocketBase&);

protected:
	int _fd;
};


class TcpSocket : public SocketBase
{
public:
	TcpSocket()
	{
		_fd = EventUtil_socket(AF_INET, SOCK_STREAM, 0);
	}
	explicit TcpSocket(int fd) { _fd = fd; }

	TcpSocket(const TcpSocket& sock) { _fd = sock.get_file_desc(); }
	TcpSocket& operator=(const TcpSocket& sock) { _fd = sock.get_file_desc(); return *this; }

	int get_file_desc() const { return _fd; }
private:
};
NAMESPACE_END
#endif
