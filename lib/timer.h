#pragma once

#include <time.h>

class Timer
{
    uint32_t m_timeout;
    uint32_t m_startTick;

public:
	Timer() : m_timeout(0), m_startTick(0) {}
	Timer(uint32_t timeout) : m_timeout(timeout), m_startTick(0) {}
	virtual ~Timer() = default;

	void start(uint32_t timeout) 
    {
        m_timeout = timeout; 
        m_startTick = Time::getTicks();
    }

	void start() { m_startTick = Time::getTicks(); }

	bool elapsed() { return ((Time::getTicks() - m_startTick) > m_timeout); }
};