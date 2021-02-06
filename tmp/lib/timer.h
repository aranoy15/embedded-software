#pragma once

#include <time.h>

class Timer
{
    uint32_t _timeout;
    uint32_t _startTick;
    bool _is_started;

public:
	Timer() : _timeout(0), _startTick(0), _is_started(false) {}

	Timer(uint32_t timeout)
	    : _timeout(timeout), _startTick(0), _is_started(false)
	{
	}

	virtual ~Timer() = default;

	void start(uint32_t timeout) 
    {
        _timeout = timeout; 
        _startTick = Time::getTicks();
    }

	void start()
	{
		_startTick = Time::getTicks();
		_is_started = true;
	}

	bool is_started() const { return _is_started; }
	void stop() { _is_started = false; }

	bool elapsed() { return ((Time::getTicks() - _startTick) > _timeout); }
};