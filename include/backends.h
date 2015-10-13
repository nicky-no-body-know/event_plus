#ifndef EVENT_PLUS_BACKENDS_H
#define EVENT_PLUS_BACKENDS_H

#define EVENT_PLUS_SELECT
#define EVENT_PLUS_POLL

//#define INIT_FDSET_SIZE 8
#define POLL_MAXLOG 1024
#include "comm.h"
#include "event_util.h"
NAMESPACE_BEGIN

class Event;
class EventBase;
enum
{
	BASE_METHOD = 0,
	SELECT_METHOD = 1
};

class BaseMethod
{
public:
	BaseMethod(EventBase& base) : _base(base) {}
	virtual int get_method_type() { return BASE_METHOD; }
	virtual void add(int fd, int flags) = 0;
	virtual void del(int fd, int flags) = 0;
	virtual int dispatch(timeval *tv) = 0;
protected:
	EventBase& _base;
};

class SelectMethod : public BaseMethod
{
public:
	SelectMethod(EventBase& base/*, int fdsize*/) : BaseMethod(base)/*, _size(fdsize)*/ { init(); }

	virtual ~SelectMethod()
	{
		// free一个空指针不会产生错误
		free(_readset_in);
		free(_writeset_in);
		free(_readset_out);
		free(_writeset_out);

		_readset_in = _writeset_in = _readset_out = _writeset_out = NULL;
	}

	virtual int get_method_type() { return SELECT_METHOD; }

	virtual void add(int fd, int flags);
	virtual void del(int fd, int flags);
	virtual int dispatch(timeval *tv);

private:
	void init()
	{
		_maxfd = -1;
		//_isResized = false;
		_readset_in = (fd_set*)malloc( sizeof(fd_set) );
		_readset_out = (fd_set*)malloc( sizeof(fd_set) );
		_writeset_in = (fd_set*)malloc( sizeof(fd_set) );
		_writeset_out = (fd_set*)malloc( sizeof(fd_set) );
		//_readset_in = (fd_set*)calloc(1, sizeof(fd_set));
		//_readset_out = (fd_set*)calloc(1, sizeof(fd_set));
		//_writeset_in = (fd_set*)calloc(1, sizeof(fd_set));
		//_writeset_out = (fd_set*)calloc(1, sizeof(fd_set));
		FD_ZERO(_readset_in);
		FD_ZERO(_readset_out);
		FD_ZERO(_writeset_in);
		FD_ZERO(_writeset_out);
	}
	//void resize(int size);

	//int _size;
	int _maxfd;
	//bool _isResized;
	fd_set *_readset_in, *_writeset_in;
	fd_set *_readset_out, *_writeset_out;
};

class PollMethod : public BaseMethod
{
public:
	PollMethod(EventBase& base) : BaseMethod(base)	{ init(); }
	virtual int get_method_type() { return SELECT_METHOD; }

	virtual void add(int fd, int flags);
	virtual void del(int fd, int flags);
	virtual int dispatch(timeval *tv);
private:
	void init() 
	{
		_pfd = (struct pollfd*)malloc(POLL_MAXLOG * sizeof(struct pollfd));
		memset(_pfd, 0x0, POLL_MAXLOG * sizeof(struct pollfd));
	}

	int _nfd;
	struct pollfd *_pfd;
};
NAMESPACE_END
#endif
