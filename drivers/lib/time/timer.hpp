#pragma once

#include <lib/time/time.hpp>

namespace lib::time
{

class Timer
{
    Time _timeout;
    Time _start;
    bool _is_started;

public:
	Timer() : _timeout(), _start(0), _is_started(false) {}

	Timer(Time timeout)
	    : _timeout(timeout), _start(0), _is_started(false)
	{
	}

	virtual ~Timer() = default;

	void start(Time timeout) 
    {
        _timeout = timeout; 
        _start = Time::current();
    }

	void start()
	{
		_start = Time::current();
		_is_started = true;
	}

	bool is_started() const { return _is_started; }
	void stop() { _is_started = false; }

	bool elapsed() { return ((Time::current() - _start) > _timeout); }
};

}
