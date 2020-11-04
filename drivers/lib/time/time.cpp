#include <lib/time/time.hpp>
#include <bsp.hpp>

using namespace lib::time;

void Time::sleep(const Time& t)
{
	if (t._time <= 0) return;

	bsp::tick::delay(t._time);
}

Time Time::current()
{
	return Time(bsp::tick::current());
}

Time& lib::time::Time::operator+=(const Time& rhs)
{
	this->_time += rhs._time;
	return *this;
}

Time& lib::time::Time::operator-=(const Time& rhs)
{
	this->_time -= rhs._time;
	return *this;
}

Time lib::time::operator-(const Time& t1, const Time& t2)
{
	return Time(t1.value() - t2.value());
}

Time lib::time::operator+(const Time& t1, const Time& t2)
{
	return Time(t1.value() + t2.value());
}

bool lib::time::operator==(const Time& t1, const Time& t2)
{
	return t1.value() == t2.value();
}

bool lib::time::operator>=(const Time& t1, const Time& t2)
{
	return t1.value() >= t2.value();
}

bool lib::time::operator<=(const Time& t1, const Time& t2)
{
	return t1.value() <= t2.value();
}

bool lib::time::operator>(const Time& t1, const Time& t2)
{
	return t1.value() > t2.value();
}

bool lib::time::operator<(const Time& t1, const Time& t2)
{
	return t1.value() < t2.value();
}

