#include "comm.h"
#include "backends.h"
#include "event.h"
#include "event_base.h"

NAMESPACE_BEGIN
using namespace std;
void EventBase::add_event(Event& event) 
{
	_ioEventMap.insert( pair<int, Event*>(event.get_file_desc(), &event) ); 
	_method->add(event.get_file_desc(), event.get_flags());
}
void EventBase::del_event(Event& event)
{
	_method->del(event.get_file_desc(), event.get_flags());
	_ioEventMap.erase( _ioEventMap.find(event.get_file_desc()) );
}
void EventBase::del_event(int fd)
{
	close(fd);
	_method->del(fd, _ioEventMap.find(fd)->second->get_flags());
	_ioEventMap.erase( _ioEventMap.find(fd) );
}

void EventBase::add_active_event(int fd, int res) 
{ 
	Event *event = _ioEventMap.find(fd)->second; 
	event->set_ret_flags( res & event->get_flags() );
	_activeEvents.push_back(event);;
}

void EventBase::dispatch()
{
	while (1)
	{
		if (_loopBreak) break;
		int res = _method->dispatch(NULL);
		if (0 == res)
		{
			handle_active_events();
		}
	}
}

void EventBase::init_methods()
{
#ifdef EVENT_PLUS_SELECT
	_methods.push_back(new SelectMethod(*this));
#endif
	vector<BaseMethod*>::iterator iter;
	for (iter = _methods.begin(); iter != _methods.end(); iter++)
	{
		_method = *iter;
		break;
	}
}

void EventBase::handle_active_events()
{
	vector<Event*>::iterator iter;
	for ( iter = _activeEvents.begin(); iter != _activeEvents.end(); iter++)
	{
		log.LOG_P(LOG_INFO, "fd = %d", (*iter)->get_file_desc());
		(*iter)->callback();	
	}
	_activeEvents.clear();
}
NAMESPACE_END
