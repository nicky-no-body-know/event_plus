#include "event_plus.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>

const char *fifo = "event.fifo";

class FifoEvent : public event_plus::Event {
public:
	FifoEvent(int fd, int type, int ncall) : 
		Event(fd, type, ncall) {}

	virtual int Callback(int fd, int res) EVENT_PLUS_UNUSED {
		char buf[1024];	
		int n = read(fd, buf, 1024 - 1);
		if (n < 0) {
			if (errno != EINTR)	{
				perror("callback");
				return -1;
			}
		}
		std::string str(buf, n);	
		std::cout << "read:" << str << endl;
	}
private:
};
int main() {
	unlink(fifo);
	// mkfifo
	int ret = mkfifo(fifo, 0600);
	if (ret < 0) {
		perror("mkfifo");
		exit(1);
	}
	// open
	int fd = open(fifo, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	Event *event = new FifoEvent(fd, EV_READ | EV_PERSIST, 1);
	EventBase::GetInstance()->AddEvent(*event);
	EventBase::GetInstance()->Dispatch();
	return 0;
}
