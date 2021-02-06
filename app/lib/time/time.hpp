#pragma once

#include <stdint.h>

namespace lib::time
{

using time_t = uint64_t;

class Time
{

public:
	enum
	{
		null_val = (time_t(1) << 63) - 1
	};

	static constexpr uint32_t second() { return 1000; }
	static constexpr uint32_t minute() { return 60 * 1000; }
	static constexpr uint32_t hour() { return 60 * minute(); }

public:
	Time() : _time(null_val) {}
	Time(const Time& t) : _time(t._time) {}
	explicit Time(time_t t) : _time(t) {}

	time_t value() const { return _time; }

    static void sleep(const Time& t);

	static Time msecs(int n) { return Time(n); }
	static Time secs(int n) { return Time(n * 1000); }
	static Time mins(int n) { return Time(n * 60 * 1000); }
	static Time current();

	Time& operator+=(const Time& rhs);
	Time& operator-=(const Time& rhs);

private:
	time_t _time;
};

Time operator-(const Time& t1, const Time& t2);
Time operator+(const Time& t1, const Time& t2);
bool operator==(const Time& t1, const Time& t2);
bool operator>=(const Time& t1, const Time& t2);
bool operator<=(const Time& t1, const Time& t2);
bool operator>(const Time& t1, const Time& t2);
bool operator<(const Time& t1, const Time& t2);

}
