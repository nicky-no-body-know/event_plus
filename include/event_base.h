#ifndef EVENT_PLUS_EVENT_BASE_H
#define EVENT_PLUS_EVENT_BASE_H

#include "comm.h"
#include "log.h"
#include "event.h"
#define LOG_PATH_NAME "../log"
#define LOG_FILE_NAME "event_plus"
NAMESPACE_BEGIN
class BaseMethod;

using namespace std;
class EventBase
{
public:
	EventBase() :_loopBreak(false) { init(); }
	~EventBase()
	{
		map<int, Event*>::iterator iter;
		for (iter = _ioEventMap.begin(); iter != _ioEventMap.end(); iter++)
			delete iter->second;
	}
	void add_event(Event& event);
	void del_event(Event& event);
	void del_event(int fd);
	void add_active_event(int fd, int res); 
	void dispatch();
	Log& get_log() { return log; }
private:
	EventBase(EventBase& base);
	EventBase& operator=(EventBase& base);

	void init() 
	{ 
		init_log(); 
		init_methods();
	}
	void init_log() { log.log_open(LOG_PATH_NAME, NULL, LOG_FILE_NAME); }
	void init_methods();
	void handle_active_events();

	bool _loopBreak;
	Log log;
	BaseMethod *_method;
	vector<BaseMethod*> _methods;	
	map<int, Event*> _ioEventMap;		
	vector<Event*> _activeEvents;
};
NAMESPACE_END
#endif
