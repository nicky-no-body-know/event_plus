#ifndef EVENT_PLUS_EVENT_UTIL_H
#define EVENT_PLUS_EVENT_UTIL_H

#define EventUtil_socket(a,b,c) NetUtil::socket(a,b,c)
#define EventUtil_bind(a,b,c) NetUtil::bind(a,b,c)
#define EventUtil_listen(a,b) NetUtil::listen(a,b)
#define EventUtil_accept(a,b,c) NetUtil::accept(a,b,c)

#define EventUtil_open(a,b,c...) FileUtil::open(a,b,##c)
#define EventUtil_fcntl_get_flag(a,b) FileUtil::fcntl_get_flag(a,b)
#define EventUtil_fcntl_set_flag(a,b,c) FileUtil::fcntl_set_flag(a,b,c)

#define EventUtil_read(a,b) IOUtil::read(a,b)
#define EventUtil_write(a,b,c) IOUtil::write(a,b,c)

#define EventUtil_malloc(a) MemUtil::malloc(a)
#define EventUtil_calloc(a,b) MemUtil::calloc(a,b)
#define EventUtil_realloc(a,b) MemUtil::realloc(a,b)

#include "comm.h"
NAMESPACE_BEGIN
class NetUtil
{
public:
	static int socket(int domain, int type, int protocol);
	static int bind(int fd, const struct sockaddr* addr, socklen_t len);
	static int listen(int fd, int backlog);
	static int accept(int fd, struct sockaddr* addr, socklen_t* len);
};

class FileUtil
{
public:
	static int open(const char* path, int oflag, ...);
	static int fcntl_get_flag(int fildes, int cmd);
	static int fcntl_set_flag(int fildes, int cmd, int flag);
};

class IOUtil
{
public:
	static int read(int fd, char *buf);
	static int write(int fd, char *buf, int len);
};

class MemUtil
{
public:
	static void* malloc(int bytes);
	static void* calloc(int counts, int size);
	static void* realloc(void *oldPtr, int size);
};
NAMESPACE_END
#endif
