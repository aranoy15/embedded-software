#pragma once

#include <bsp.h>
#include <datetime.h>
#include <singleton.h>

class DateTimeManager : public Singleton<DateTimeManager>
{
private:
    RTC_HandleTypeDef mHrtc; 

public:

    DateTimeManager() : mHrtc() {}
    virtual ~DateTimeManager() {}

    void init();
    void deinit();

    DateTime now();
    void set(const DateTime& dateTime);
    void setDate(const DateTime& dateTime);
};
