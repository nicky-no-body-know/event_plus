#ifndef EVENT_PLUS_INCLUDE_EVENT_PLUS_H
#define EVENT_PLUS_INCLUDE_EVENT_PLUS_H

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/select.h>
#include <errno.h>
#include "event_plus_internal.h"
#define DISALLOW_COPY_AND_ASSIGN(type) \
	type(const type&); \
	void operator=(const type&);

#if defined(__GNUC__) && !defined(COMPILER_ICC)
#define EVENT_PLUS_UNUSED __attribute__ ((unused))
#else
#define EVENT_PLUS_UNUSED
#endif

// 定义Event类型
#define EV_READ 0x01
#define EV_WRITE 0x02
#define EV_SIGNAL 0x04
#define EV_TIMEOUT 0x08
#define EV_PERSIST 0x10

// 定义Event的状态
#define EVLIST_INIT 0x01
#define EVLIST_IO 0x02
#define EVLIST_SIGNAL 0x04
#define EVLIST_TIMEOUT 0x08
#define EVLIST_ACTIVE 0x10

namespace event_plus { // namespace event_plus start

class Event;

typedef event_plus::internal::ListNode<Event&> EventNode;

class Event {
public:
	Event(int fd, int type, int ncall) 
		: fd_(fd), 
		  type_(type), 
		  status_(EVLIST_INIT),
		  res_(0),
		  ncall_(ncall),
		  tv(NULL){}

	virtual ~Event() {
		if (tv) delete tv;
	}

	//用户必须继承这个类并定义该回调函数以实现自己的业务逻辑
	virtual int Callback(int fd, int res) EVENT_PLUS_UNUSED { return 0; }

	int fd() { return fd_; }
	int fd() const { return fd_; }

	int type() { return type_; }
	int type() const { return type_; }
	void set_type(int type) { type_ = type; }

	int status() { return status_; }
	int status() const { return status_; }
	void set_status(int status) { status_ = status; }

	int res() { return res_; }
	void set_res(int res) { res_ = res; }
	int ncall() { return ncall_; }
	struct timeval* timeout() { return tv; }

private:
	friend class EventBase;
	friend class event_plus::internal::List<Event&>;
	friend class event_plus::internal::List<Event*>;
	
	void set_io_container(const EventNode *container) {
		io_container_ = container; 
	}
	const EventNode *get_io_container() { return io_container_; }

	void set_signal_container(const EventNode *container) { 
		signal_container_ = container; 
	}
	const EventNode *get_signal_container() { return signal_container_; }

	void set_timeout_container(const EventNode *container) { 
		timeout_container_ = container; 
	}
	const EventNode *get_timeout_container() { return timeout_container_; }

	void set_active_container(const EventNode *container) { 
		active_container_ = container; 
	}
	const EventNode *get_active_container() { return active_container_; }

	int fd_;
	int type_;  // Event的类型
	int status_; // Event的状态
	int res_; // Event的触发类型
	int ncall_; // Event的回调次数
	struct timeval *tv; // 超时时间
	const EventNode *io_container_;
	const EventNode *timeout_container_;
	const EventNode *signal_container_;
	const EventNode *active_container_;
};


// method
class MethodInterface {
public:
	MethodInterface() : events() {}
	//向后端方法添加一个事件，如果返回值小于0，则调用方应该释放该对象
	virtual int AddEvent(const Event &event) { return -1; };
	virtual int DelEvent(const Event &event) { return -1; };
	virtual int Dispatch(const int flag, timeval *tv) { return -1; };

protected:
	int SaveEventPointer(const Event &event);
	void CheckShouldRemoveEvent(const Event *event) {
		if (!(event->type() & EV_PERSIST)) {
			DelEvent(*event);
		}
	}
	std::vector<Event*> events;
};

class SelectMethod :public MethodInterface{
public:
	SelectMethod();
	~SelectMethod();

	//添加一个事件，type是事件类型
	virtual int AddEvent(const Event &event);
	virtual int DelEvent(const Event &event);
	// flag暂时没有用到
	virtual int Dispatch(const int flag, struct timeval *tv);

private:
	void check_max_fd(int fd) {
		if (fd > maxfd_) {
			maxfd_ = fd;
		}
	}
	void add_to_fdset(int fd, int type) {
		if (type & EV_READ) {
			FD_SET(fd, readset_in_);
		}
		if (type & EV_WRITE) {
			FD_SET(fd, writeset_in_);
		}
	}
	void delete_from_fdset(int fd, int type) {
		if (type & EV_READ) {
			FD_CLR(fd, readset_in_);
		}
		if (type & EV_WRITE) {
			FD_CLR(fd, writeset_in_);
		}
	}
	int compute_event_res(int fd) {
		int res = 0;
		if (FD_ISSET(fd, readset_out_)) {
			res |= EV_READ;
		}
		if (FD_ISSET(fd, writeset_out_)) {
			res |= EV_WRITE;
		}
		return res;
	}
	int maxfd_;
	fd_set *readset_in_; 
	fd_set *writeset_in_;
	fd_set *readset_out_;
	fd_set *writeset_out_;
};

// base
class EventBase {
public:
	static EventBase* GetInstance();
	~EventBase();

	int AddEvent(Event &event);
	int AddIOEvent(Event &event);
	int AddSignalEvent(Event &event);
	int AddTimeoutEvent(Event &event);
	int AddActiveEvent(Event &event);
	void ClearAll();
	
	void Dispatch(const int flag);
	
	bool has_active() { return !active_event_list_.empty(); }
	bool has_io() { return !io_event_list_.empty(); }
	bool has_signal() { return !signal_event_list_.empty(); }
	bool has_timeout() { return !timemout_event_list_.empty(); }
	bool has_events() { return has_io() || 
							   has_signal() ||
							   has_timeout() ||
	                           has_active(); }
private:
	EventBase();
	void ProcessActive();
	inline void clear_queue(event_plus::internal::List<Event&>& queue);

	event_plus::internal::List<Event&> io_event_list_;
	event_plus::internal::List<Event&> signal_event_list_;
	event_plus::internal::List<Event&> timemout_event_list_;
	event_plus::internal::List<Event&> active_event_list_;

	MethodInterface *method_;
	DISALLOW_COPY_AND_ASSIGN(EventBase);
};

} // namespace event_plus end
#endif
