#include "comm.h"
#include "event.h"
#include "event_base.h"
#include "listener.h"
#include "log.h"

#define SERVER_PORT 56000

using namespace event_plus;
char buf[BUFLEN];
struct HelloCallback : public EventCallback
{
	HelloCallback(EventBase& base) : _base(base) { }
	virtual int callback(int fd, int res) const
	{
		if ((res & EV_READ) && (res & EV_WRITE))
		{
			int len = ::read(fd, buf, BUFLEN);
			if (0 == len)
			{
				_base.del_event(fd);
			} else
			{
				printf("hello : %s", buf);
				::write(fd, buf, len);
			}
		}
		return 0;
	}
private:
	EventBase& _base;
};

int main()
{
	struct sockaddr_in addr;
	EventBase base;
	TcpSocket listen_sock(base);
	HelloCallback hello(base);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	ServerListener listener(base, listen_sock, (struct sockaddr*)&addr, &hello);	
	Event lev(listen_sock.get_file_desc(), &listener, EV_READ);
	base.add_event(lev);

	base.dispatch();
	
	return 0;
}
