#ifndef EVENT_PLUS_LISTENER_H
#define EVENT_PLUS_LISTENER_H

#define MAX_BACKLOG 100

#include "comm.h"
#include "socket.h"
#include "event_base.h"
#include "event.h"
#include "log.h"

NAMESPACE_BEGIN

class Event;
class TcpSocket;

class ServerListener : public EventCallback
{
public:
	ServerListener(EventBase& base, const TcpSocket& sock, const sockaddr *addr, EventCallback *cb) :
		_base(base), _sock(sock), _addr(addr), _cb(cb)
	{
		Log& log = _base.get_log();
		log.LOG_P(LOG_INFO, "sockfd = %d", _sock.get_file_desc());
		::bind(_sock.get_file_desc(), _addr, sizeof(*addr));
		::listen(_sock.get_file_desc(), MAX_BACKLOG);	
	}

	virtual int callback(int fd, int res) const;
private:
	ServerListener(const ServerListener&);
	ServerListener& operator=(const ServerListener&);

	EventBase& _base;
	const TcpSocket& _sock;	
	const sockaddr *_addr;
	EventCallback *_cb;
};
NAMESPACE_END
#endif
