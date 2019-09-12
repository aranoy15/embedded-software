#pragma once

#include <stdint.h>

typedef uint64_t TInt;

class Time
{
private:
	TInt m_time;

public:
	enum
	{
		nullVal = (uint64_t(1) << 63) - 1
	};
	//static const uint32_t t_infinite = osWaitForever;

	static constexpr uint32_t second() { return 1000; }
	static constexpr uint32_t minute() { return 60 * 1000; }
	static constexpr uint32_t hour() { return 60 * minute(); }

public:
	Time() : m_time(nullVal) {}
	Time(const Time& t) : m_time(t.m_time) {}
	explicit Time(TInt t) : m_time(t) {}

    static void sleep(const Time& t);

	static Time milliSeconds(int n) { return Time(n); }
	static Time seconds(int n) { return Time(n * 1000); }
	static uint32_t getTicks();
};
