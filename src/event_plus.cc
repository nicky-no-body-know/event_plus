#include "event_plus_internal.h"
#include "event_plus.h"

namespace event_plus { //namespace event_plus start

////////////////////////////SelectMethod/////////////////////////
int MethodInterface::SaveEventPointer(const Event &event) {
	int fd = event.fd();
	if (fd < 0) {
		return -1;
	}

	if (events.size() > static_cast<unsigned long>(fd)) {	
		if (events[fd]) {
			events[fd]->set_type(events[fd]->type() | event.type());
			return -1;
		}
		events[fd] =  const_cast<Event*>(&event);
		return 0;
	}
	events.resize(fd + 1);
	events[fd] = const_cast<Event*>(&event);
	return 0;
}
SelectMethod::SelectMethod() : MethodInterface(), maxfd_(-1) {
	readset_in_ = (fd_set*)malloc(sizeof(fd_set));
	writeset_in_ = (fd_set*)malloc(sizeof(fd_set));
	readset_out_ = (fd_set*)malloc(sizeof(fd_set));
	writeset_out_ = (fd_set*)malloc(sizeof(fd_set));

	memset(readset_in_, 0, sizeof(fd_set));
	memset(writeset_in_, 0, sizeof(fd_set));
	memset(readset_out_, 0, sizeof(fd_set));
	memset(writeset_out_, 0, sizeof(fd_set));
}

SelectMethod::~SelectMethod() {
	delete readset_in_;
	delete writeset_in_;
	delete readset_out_;
	delete writeset_out_;
}

int SelectMethod::AddEvent(const Event &event) {
	int fd = event.fd();
	int type = event.type();
	if (fd < 0 ) {
		return -1;
	}
	
	check_max_fd(fd);
	add_to_fdset(fd, type);
	return SaveEventPointer(event);	
}

int SelectMethod::DelEvent(const Event &event) {
	delete_from_fdset(event.fd(), event.type());
	events[event.fd()] = NULL;
	return 0;
}

int SelectMethod::Dispatch(int flag, struct timeval *tv) {

	memcpy((char*)readset_out_, readset_in_, sizeof(fd_set));
	memcpy((char*)writeset_out_, writeset_in_, sizeof(fd_set));

	int nfd = maxfd_ + 1;
	int ret = ::select(nfd, readset_out_, writeset_out_, NULL, tv);
	if (ret < 0) {
		if (errno != EINTR) {
			perror("select");
		}
		return ret;
	}
	
	for (int i = 0; i < nfd; i++) {
		int res = compute_event_res(i);
		CheckShouldRemoveEvent(events[i]);
		events[i]->set_res(res | events[i]->res());
		if (res > 0) {
			EventBase::GetInstance()->AddActiveEvent(*events[i]);
		}
	}
	return ret;
}
/////////////////////////SelectMethod/////////////////////////
/////////////////////////EventBase////////////////////////////
EventBase* EventBase::GetInstance() {
	static EventBase event_base;
	return &event_base;	
}

EventBase::EventBase()
	:io_event_list_(),
	 signal_event_list_(),
	 timemout_event_list_(),
	 active_event_list_() {
	method_ = new SelectMethod();
}

EventBase::~EventBase() {
	ClearAll();
}

int EventBase::AddEvent(Event &event) {
	if ( (event.type() & (EV_READ | EV_WRITE)) &&
		 ( event.type() & EV_SIGNAL ) ) {
		// log error return -1;	
	}
	//Timeout event
	if (event.type() & EVLIST_TIMEOUT) {
		//TODO: add timeout event handle
		return -1;
	}
	// IO event
	if ( !( event.status() & (EVLIST_IO | EVLIST_ACTIVE) ) 
			&& (event.type() & (EV_READ | EV_WRITE)) ) {
		return AddIOEvent(event);
	}
	//TODO:add signal event
	return 0;
}

int EventBase::AddIOEvent(Event &event) {
	event.set_status(event.status() & EVLIST_IO);
	event.set_io_container( io_event_list_.InsertTail(event) );
	if (method_) {
		return method_->AddEvent(event);
	}
	return -1;
}

int EventBase::AddSignalEvent(Event &event) {
	return -1;
}

int EventBase::AddTimeoutEvent(Event &event) {
	return -1;
}

int EventBase::AddActiveEvent(Event &event) {
	event.set_status(event.status() & EVLIST_ACTIVE);
	event.set_active_container( active_event_list_.InsertTail(event) );
	return 0;
}

void EventBase::clear_queue(event_plus::internal::List<Event&> &list) {
	while (!list.empty()) {
		Event &event = list.PopFront();
		delete &event;
	}
}

void EventBase::ClearAll() {
	clear_queue(io_event_list_);
	clear_queue(signal_event_list_);
	clear_queue(timemout_event_list_);
	clear_queue(active_event_list_);
}

void EventBase::Dispatch(int flag) {
	int done = 0;
	while (!done) {
		if (!has_events()) {
			// log error
			return;
		}
		int ret = method_->Dispatch(0, NULL);
		if (ret > 0) {
			ProcessActive();
		}
	}
}

void EventBase::ProcessActive() {
	while (!active_event_list_.empty()) {
		Event &event = active_event_list_.PopFront();
		event.Callback(event.fd(), event.res());
	}
}
//////////////////////////////EventBase//////////////////////////

namespace internal { // namespace internal start
#define LIST_INSERT_TAIL(t) \
do { \
	Node *node = new Node(t); \
	if (empty()) { \
		head_ = node; \
		return 0; \
	} \
	if (1 == count()) { \
		tail_ = node; \
		head_->set_next(tail_); \
		tail_->set_prev(head_); \
		return 0; \
	} \
	tail_->set_next(node); \
	node->set_prev(tail_); \
	tail_ = node; \
	count_++; \
	return node; \
} while (0);

#define LIST_POP_FRONT_TYPE(type) \
do { \
	head_->next()->set_prev(NULL); \
	type ret = head_->data(); \
	Node* old = head_; \
	head_ = head_->next(); \
	delete old; \
	return ret; \
} while (0);

#define LIST_REMOVE(node) \
do { \
	node->next()->set_prev(node->prev()); \
	node->prev()->set_next(node->next()); \
	node->clear_data(); \
	delete node; \
	return 0; \
} while(0);

// 在List尾端插入元素，同时回调
template <class T>
ListNode<T>* List<T>::InsertTail(T& t) {
	LIST_INSERT_TAIL(t);
}

// 从头端取出一个元素，需要调用者释放该数据
template <class T>
T& List<T>::PopFront() {
	if (empty()) {
		// log error
		exit(1);
	}
	LIST_POP_FRONT_TYPE(reference_type);	
}

// 删除一个元素，数据自动释放
template <class T>
int List<T>::Remove(ListNode<T>* node) {
	if (empty() || !node) {
		// log error
		return -1;
	}
	LIST_REMOVE(node);
}

// 在List尾端插入元素，同时回调
template <class T>
ListNode<T>* List<T*>::InsertTail(pointer_type t) {
	LIST_INSERT_TAIL(t);
}

// 从头端取出一个元素，需要调用者释放该数据
template <class T>
T* List<T*>::PopFront() {
	if (empty()) {
		//log error
		return NULL;
	}
	LIST_POP_FRONT_TYPE(pointer_type);
}

// 删除一个元素，数据自动释放
template <class T>
int List<T*>::Remove(Node *node) {
	if (empty() || !node) {
		return -1;
	}
	LIST_REMOVE(node);
}

} // namespace internal end
} // namespace event_plus end
