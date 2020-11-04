#ifndef DRIVERS_LIB_TASK_TASKBASE
#define DRIVERS_LIB_TASK_TASKBASE

#include <lib/task/priority.hpp>
#include <lib/time/timer.hpp>

namespace lib::task
{

class TaskBase
{
public:
    using time_t = lib::time::Time;
    using timer_t = lib::time::Timer;
    using prio_t = Priority;
public:
	TaskBase(time_t period, prio_t prio)
	    : _period(period), _prio(prio), _timer()
	{
		_timer.start(_period);
	}

	virtual ~TaskBase() = default;

	virtual void setup() = 0;
    virtual void func() = 0;

	prio_t prio() const { return _prio; }
    time_t period() const { return _period; }
    timer_t& timer() { return _timer; }

protected:
    time_t _period; 
    prio_t _prio;
    timer_t _timer;

};

}

#endif /* DRIVERS_LIB_TASK_TASKBASE */
