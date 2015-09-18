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
		int len = 0;
		Log &log = _base.get_log();
		log.log_p(LOG_INFO, "%s", "hello callback");
		char hello[] = "hello\n";
		char world[] = "world\n";
		if ((res & EV_READ) && (res & EV_WRITE))
		{
			len = EventUtil_read(fd, buf);
			if (0 == len)
			{
				_base.del_event(fd);
			}
			EventUtil_write(fd, hello, sizeof(hello));
			EventUtil_write(fd, buf, len);
			EventUtil_write(fd, world, sizeof(world));
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
	TcpSocket listen_sock;
	HelloCallback hello(base);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	ServerListener listener(base, listen_sock, (const struct sockaddr*)&addr, &hello);	
	Event lev(listen_sock.get_file_desc(), &listener, EV_READ);
	base.add_event(lev);

	base.dispatch();
	
	return 0;
}
