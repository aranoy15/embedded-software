#pragma once

#include <lib/datetime/timespan.hpp>
#include <lib/stream/stream.hpp>
#include <string>

namespace lib::datetime
{

class DateTime
{
private:
    static const uint32_t secondsFrom1970To2000 = 946684800;
	static const uint8_t daysInMonth[];

private:
	static uint16_t date2days(uint16_t y, uint8_t m, uint8_t d)
	{
		if (y >= 2000) y -= 2000;
		uint16_t days = d;
		for (uint8_t i = 1; i < m; ++i)
			days += daysInMonth[i - 1];
		if (m > 2 && y % 4 == 0) ++days;
		return days + 365 * y + (y + 3) / 4 - 1;
	}

	static long time2long(uint16_t days, uint8_t h, uint8_t m, uint8_t s)
	{
		return ((days * 24L + h) * 60 + m) * 60 + s;
	}

protected:
    uint8_t _year;
    uint8_t _month;
    uint8_t _day;
    uint8_t _hour;
    uint8_t _minute;
    uint8_t _second;

public:
	DateTime(uint32_t t = 0);
	DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour = 0,
	         uint8_t min = 0, uint8_t sec = 0);
	DateTime(const DateTime& copy);
	DateTime(const char *date, const char *time);
	DateTime(const char *dateTime);

	uint16_t full_year() const { return 2000 + _year; }
	uint8_t year() const { return _year; }
	uint8_t month() const { return _month; }
	uint8_t day() const { return _day; }
	uint8_t hour() const { return _hour; }
	uint8_t minute() const { return _minute; }
	uint8_t second() const { return _second; }
	uint8_t day_of_the_week() const;

	/*
	std::string toString() const {
		return utils::stringFormat("%02u.%02u.%04u %02u:%02u:%02u", day(),
		                           month(), year(), hour(), minute(), second());
	}

	std::string timeString(std::string format = "") {
		if (format.empty()) 
			format = "%02u:%02u:%02u";
		
		return utils::stringFormat(format, hour(), minute(), second());
	}
	*/

	// 32-bit times as seconds since 1/1/2000
	long secondstime() const;
	// 32-bit times as seconds since 1/1/1970
	uint32_t unixtime() const;

	DateTime operator+(const TimeSpan& span);
	DateTime operator-(const TimeSpan& span);
	TimeSpan operator-(const DateTime& right);
};

lib::stream::Stream& operator<<(lib::stream::Stream& stream, const DateTime& datetime);

}