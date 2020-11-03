#include <lib/datetime/timespan.hpp>

using namespace lib::datetime;

TimeSpan::TimeSpan(uint32_t seconds) : m_seconds(seconds) {}

TimeSpan::TimeSpan(uint16_t days, uint8_t hours, uint8_t minutes,
                   uint8_t seconds)
    : m_seconds(days * 86400L + hours * 3600 + minutes * 60 + seconds)
{
}

TimeSpan::TimeSpan(const TimeSpan& copy) : m_seconds(copy.m_seconds) {}

TimeSpan TimeSpan::operator+(const TimeSpan& right)
{
	return TimeSpan(m_seconds + right.m_seconds);
}

TimeSpan TimeSpan::operator-(const TimeSpan& right)
{
	return TimeSpan(m_seconds - right.m_seconds);
}