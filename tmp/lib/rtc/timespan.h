#pragma once

#include <inttypes.h>

class TimeSpan
{
protected:
	uint32_t m_seconds;

public:
    TimeSpan(uint32_t seconds);
    TimeSpan(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds);
    TimeSpan(const TimeSpan& copy);

	uint16_t days() const { return m_seconds / 86400L; }
	uint8_t hours() const { return m_seconds / 3600 % 24; }
	uint8_t minutes() const { return m_seconds / 60 % 60; }
	uint8_t seconds() const { return m_seconds % 60; }
	uint32_t totalSeconds() const { return m_seconds; }

    TimeSpan operator+(const TimeSpan& right);
    TimeSpan operator-(const TimeSpan& right);
};