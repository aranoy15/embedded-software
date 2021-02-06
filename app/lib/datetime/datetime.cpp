#include <lib/datetime/datetime.hpp>
#include <lib/utils.hpp>
#include <lib/stream/actions.hpp>

using namespace lib::datetime;

const uint8_t DateTime::daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

DateTime::DateTime(uint32_t t)
    : _year(0), _month(0), _day(0), _hour(0), _minute(0), _second(0)
{
    t -= secondsFrom1970To2000;    // bring to 2000 timestamp from 1970

    _second = t % 60;
    t /= 60;
    _minute = t % 60;
    t /= 60;
    _hour = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (_year = 0; ; ++_year) {
        leap = _year % 4 == 0;
        if (days < 365 + leap)
            break;
        days -= 365 + leap;
    }
    for (_month = 1; ; ++_month) {
        uint8_t daysPerMonth = daysInMonth[_month - 1];
        if (leap && _month == 2)
            ++daysPerMonth;
        if (days < daysPerMonth)
            break;
        days -= daysPerMonth;
    }
    _day = days + 1; 
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                   uint8_t min, uint8_t sec)
    : _year(0), _month(0), _day(0), _hour(0), _minute(0), _second(0)
{
	if (year >= 2000) year -= 2000;
	_year = year;
	_month = month;
	_day = day;
	_hour = hour;
	_minute = min;
	_second = sec;
}

DateTime::DateTime(const DateTime& copy)
    : _year(copy._year),
      _month(copy._month),
      _day(copy._day),
      _hour(copy._hour),
      _minute(copy._minute),
      _second(copy._second)
{
}

DateTime::DateTime(const char *date, const char *time)
    : _year(0), _month(0), _day(0), _hour(0), _minute(0), _second(0)
{
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    _year = lib::utils::convert_to_date(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
    switch (date[0]) {
        case 'J': _month = date[1] == 'a' ? 1 : _month = date[2] == 'n' ? 6 : 7; break;
        case 'F': _month = 2; break;
        case 'A': _month = date[2] == 'r' ? 4 : 8; break;
        case 'M': _month = date[2] == 'r' ? 3 : 5; break;
        case 'S': _month = 9; break;
        case 'O': _month = 10; break;
        case 'N': _month = 11; break;
        case 'D': _month = 12; break;
    }

    _day = lib::utils::convert_to_date(date + 4);
    _hour = lib::utils::convert_to_date(time);
    _minute = lib::utils::convert_to_date(time + 3);
    _second = lib::utils::convert_to_date(time + 6);
}

DateTime::DateTime(const char* dateTime)
    : _year(0), _month(0), _day(0), _hour(0), _minute(0), _second(0)
{
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	_year = lib::utils::convert_to_date(dateTime + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
    switch (dateTime[0]) {
        case 'J': _month = dateTime[1] == 'a' ? 1 : _month = dateTime[2] == 'n' ? 6 : 7; break;
        case 'F': _month = 2; break;
        case 'A': _month = dateTime[2] == 'r' ? 4 : 8; break;
        case 'M': _month = dateTime[2] == 'r' ? 3 : 5; break;
        case 'S': _month = 9; break;
        case 'O': _month = 10; break;
        case 'N': _month = 11; break;
        case 'D': _month = 12; break;
    }

    _day = lib::utils::convert_to_date(dateTime + 4);
    _hour = lib::utils::convert_to_date(dateTime + 12);
    _minute = lib::utils::convert_to_date(dateTime + 15);
    _second = lib::utils::convert_to_date(dateTime + 18);
}

uint8_t DateTime::day_of_the_week() const {    
    uint16_t day = date2days(_year, _month, _day);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

long DateTime::secondstime(void) const {
  uint16_t days = date2days(_year, _month, _day);
  return time2long(days, _hour, _minute, _second);
}

DateTime DateTime::operator+(const TimeSpan& span)
{
	return DateTime(unixtime() + span.totalSeconds());
}

DateTime DateTime::operator-(const TimeSpan& span)
{
	return DateTime(unixtime() - span.totalSeconds());
}

TimeSpan DateTime::operator-(const DateTime& right)
{
	return TimeSpan(unixtime() - right.unixtime());
}

lib::stream::Stream& lib::datetime::operator<<(lib::stream::Stream& stream, const DateTime& datetime)
{
    using namespace lib::stream;

	stream << IntWidthAction(2) << JustifyAction(Stream::Justify::Right);
	stream << datetime.day() << "." << datetime.month() << ".";
	stream << IntWidthAction(4);
	stream << datetime.full_year() << " ";
	stream << IntWidthAction(2);
	stream << datetime.hour() << ":" << datetime.minute() << ":" << datetime.second();

    return stream;
}
