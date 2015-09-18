#include "event_util.h"
NAMESPACE_BEGIN
int NetUtil::socket(int domain, int type, int protocol)
{
	int ret;
	if ( -1 == (ret = ::socket(domain, type, protocol)) ) 
	{
		perror("socket create failed!");
		exit(1);
	}
	return ret;
}

int NetUtil::bind(int fd, const struct sockaddr* addr, socklen_t len)
{
	int ret;
	if (-1 == (ret = ::bind(fd, addr, len)))
	{
		perror("bind failed!");
		exit(1);
	}
	return ret;
}
int NetUtil::listen(int fd, int backlog)
{
	int ret;
	if (-1 == (ret = ::listen(fd, backlog)))
	{
		perror("listen failed!");
		exit(1);
	}
	return ret;
}
int NetUtil::accept(int fd, struct sockaddr* addr, socklen_t* len)
{
	int ret;
	if (-1 == (ret = ::accept(fd, addr, len)))
	{
		perror("accept failed!");
		exit(1);
	}
	return ret;
}

int FileUtil::open(const char *path, int oflag, ...)
{
	int ret;
	va_list args;
	va_start(args, oflag);
	if (-1 == (ret = ::open(path, oflag, args)) )
	{
		perror("open failed!");
		exit(1);
	}
	va_end(args);
	return ret;
}
int FileUtil::fcntl_get_flag(int fildes, int cmd)
{
	int ret;
	if ( -1 == (ret = ::fcntl(fildes, cmd)))
	{
		perror("fcntl get flag failed");
		exit(1);
	}
	return ret;
}

int FileUtil::fcntl_set_flag(int fildes, int cmd, int flag)
{
	int ret;
	if (-1 == (ret = ::fcntl(fildes, cmd, flag)))
	{
		perror("fcntl set flag failed");
		exit(1);
	}
	return ret;
}

int IOUtil::read(int fd, char *buf)
{
	int ret;
	if ( -1 == (ret = ::read(fd, buf, BUFLEN)) )
	{
		perror("raed error");
		exit(1);
	}
	return ret;
}

int IOUtil::write(int fd, char *buf, int len)
{
	int ret;
	int hasWrite = 0;	
	int left = len;
	while (left)
	{
		if (-1 == (ret = ::write(fd, buf + hasWrite, left)))
		{
			perror("write failed!");
			exit(1);
		}
		hasWrite += ret;
		left -= ret;
	}
	return 0;
}

void* MemUtil::malloc(int bytes)
{
	void *ptr;
	if (NULL == (ptr = ::malloc(bytes)) )
	{
		perror("malloc failed!");
		exit(1);
	}
	return ptr;
}
void* MemUtil::calloc(int counts, int size)
{
	void *ptr;
	if (NULL == (ptr = ::calloc(counts, size)) )
	{
		perror("calloc failed!");
		exit(1);
	}
	return ptr;
}
void* MemUtil::realloc(void *oldPtr, int size)
{
	void *ptr;
	if (NULL == (ptr = ::realloc(oldPtr, size)) )
	{
		perror("realloc failed!");
		exit(1);
	}
	return ptr;
}
NAMESPACE_END
