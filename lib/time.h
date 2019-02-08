#ifndef TIME_H
#define TIME_H

#include <cmsis_os.h>
#include <stdint.h>
#include <stm32f1xx_hal.h>

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
	static const uint32_t t_infinite = osWaitForever;

public:
	Time() : m_time(nullVal) {}
	Time(const Time& t) : m_time(t.m_time) {}
	explicit Time(TInt t) : m_time(t) {}

    static void Sleep(const Time& t);

	static Time MilliSeconds(int n) { return Time(n); }
	static Time Seconds(int n) { return Time(n * 1000); }
	static uint32_t GetTicks();
};

#endif /* TIME_H */
