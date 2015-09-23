#include "backends.h"
#include "event.h"
#include "event_util.h"
#include "event_base.h"
#define log _base.get_log()
NAMESPACE_BEGIN
using namespace std;
void SelectMethod::add(int fd, int flags)
{
	assert(fd >= 0);
	int tmpfd = fd;
	if (tmpfd > _maxfd)
	{
		_maxfd = tmpfd;
		if (_size < _maxfd + 1)
		{
			_size *= 2;
			resize(_size);
		}
	}
	if (flags & EV_READ)
		FD_SET(tmpfd, _readset_in);
	if (flags & EV_WRITE)
		FD_SET(tmpfd, _writeset_in);
	log.LOG_P(LOG_INFO, "maxfd=%d, size=%d", _maxfd, _size);
}

void SelectMethod::del(int fd, int flags)
{
	assert(fd >= 0);
	int tmpfd = fd;
	if (EV_READ & flags)
		FD_CLR(tmpfd, _readset_in);
	if (EV_WRITE & flags)
		FD_CLR(tmpfd, _writeset_in);
}


int SelectMethod::dispatch(timeval *tv)
{
	void *oldPtr;
	int nfd;
	int res;

	if (_isResized)
	{
		oldPtr = _readset_out;
		_readset_out = (fd_set*)EventUtil_realloc(oldPtr, _size );
		free(oldPtr);

		oldPtr = _writeset_in;
		_writeset_in = (fd_set*)EventUtil_realloc(oldPtr, _size );
		free(oldPtr);
		_isResized = false;
	}
	memcpy((void*)_readset_out, (void*)_readset_in, _size);
	memcpy((void*)_writeset_out, (void*)_writeset_in, _size);
	
	nfd = _maxfd + 1;
	res = ::select(nfd, _readset_out, _writeset_out, NULL, tv);
	if (-1 == res)
	{
		perror("select faild!");
		return -1;
	}
	log.LOG_P(LOG_INFO, "nfd = %d, res = %d", nfd, res);
	vector<int> fds;
	for (int i = 0; i < nfd; i++)
	{
		int flag_res = 0;
		if (FD_ISSET(i, _readset_out))
		{
			flag_res |= EV_READ;
		}
		if (FD_ISSET(i, _writeset_out))
		{
			flag_res |= EV_WRITE;
		}
		if (0 != res)
			_base.add_active_event(i, flag_res);
	}
	return 0;
}
void SelectMethod::resize(int size)
{
	assert(size > 0);
	_isResized = true;
	EventUtil_realloc(_readset_in, size );
	EventUtil_realloc(_writeset_in, size );
}


NAMESPACE_END

