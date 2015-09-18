#include "backends.h"
#include "event.h"
#include "event_util.h"
#include "event_base.h"

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
	Log& log = _base.get_log();
	log.log_p(LOG_INFO, "%s, maxfd=%d, size=%d", "select add", _maxfd, _size);
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
		Log &log = _base.get_log();
		log.log_p(LOG_INFO, "%s", "resize");
		oldPtr = _readset_out;
		_readset_out = (fd_set*)EventUtil_realloc(oldPtr, _size * sizeof(fd_set));
		free(oldPtr);

		oldPtr = _writeset_in;
		_writeset_in = (fd_set*)EventUtil_realloc(oldPtr, _size * sizeof(fd_set));
		free(oldPtr);
		_isResized = false;
	}
	
	nfd = _maxfd + 1;
	res = ::select(nfd, _readset_out, _writeset_out, NULL, tv);
	if (-1 == res)
	{
		perror("select faild!");
		return -1;
	}

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
	EventUtil_realloc(_readset_in, size * sizeof(fd_set));
	EventUtil_realloc(_writeset_in, size * sizeof(fd_set));
}


NAMESPACE_END
