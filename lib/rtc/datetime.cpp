#include <datetime.h>
#include <utils.h>

const uint8_t DateTime::daysInMonth[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

DateTime::DateTime(uint32_t t)
    : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0), m_second(0)
{
    t -= secondsFrom1970To2000;    // bring to 2000 timestamp from 1970

    m_second = t % 60;
    t /= 60;
    m_minute = t % 60;
    t /= 60;
    m_hour = t % 24;
    uint16_t days = t / 24;
    uint8_t leap;
    for (m_year = 0; ; ++m_year) {
        leap = m_year % 4 == 0;
        if (days < 365 + leap)
            break;
        days -= 365 + leap;
    }
    for (m_month = 1; ; ++m_month) {
        uint8_t daysPerMonth = daysInMonth[m_month - 1];
        if (leap && m_month == 2)
            ++daysPerMonth;
        if (days < daysPerMonth)
            break;
        days -= daysPerMonth;
    }
    m_day = days + 1; 
}

DateTime::DateTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour,
                   uint8_t min, uint8_t sec)
    : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0), m_second(0)
{
	if (year >= 2000) year -= 2000;
	m_year = year;
	m_month = month;
	m_day = day;
	m_hour = hour;
	m_minute = min;
	m_second = sec;
}

DateTime::DateTime(const DateTime& copy)
    : m_year(copy.m_year),
      m_month(copy.m_month),
      m_day(copy.m_day),
      m_hour(copy.m_hour),
      m_minute(copy.m_minute),
      m_second(copy.m_second)
{
}

DateTime::DateTime(const char *date, const char *time)
    : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0), m_second(0)
{
    // sample input: date = "Dec 26 2009", time = "12:34:56"
    m_year = utils::conv2d(date + 9);
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec 
    switch (date[0]) {
        case 'J': m_month = date[1] == 'a' ? 1 : m_month = date[2] == 'n' ? 6 : 7; break;
        case 'F': m_month = 2; break;
        case 'A': m_month = date[2] == 'r' ? 4 : 8; break;
        case 'M': m_month = date[2] == 'r' ? 3 : 5; break;
        case 'S': m_month = 9; break;
        case 'O': m_month = 10; break;
        case 'N': m_month = 11; break;
        case 'D': m_month = 12; break;
    }

    m_day = utils::conv2d(date + 4);
    m_hour = utils::conv2d(time);
    m_minute = utils::conv2d(time + 3);
    m_second = utils::conv2d(time + 6);
}

uint8_t DateTime::dayOfTheWeek() const {    
    uint16_t day = date2days(m_year, m_month, m_day);
    return (day + 6) % 7; // Jan 1, 2000 is a Saturday, i.e. returns 6
}

long DateTime::secondstime(void) const {
  uint16_t days = date2days(m_year, m_month, m_day);
  return time2long(days, m_hour, m_minute, m_second);
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