#include <datetimemanager.h>

void DateTimeManager::init()
{
	mHrtc.Instance = RTC;
	mHrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	mHrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
    bsp::rtc::init();
	HAL_RTC_Init(&mHrtc);
}

void DateTimeManager::deinit()
{
    bsp::rtc::deinit();
    HAL_RTC_DeInit(&mHrtc);
}

DateTime DateTimeManager::now()
{
    RTC_TimeTypeDef time;
    RTC_DateTypeDef date; 
	DateTime result;

	HAL_RTC_GetTime(&mHrtc, &time, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&mHrtc, &time, RTC_FORMAT_BIN);

	HAL_RTC_GetDate(&mHrtc, &date, RTC_FORMAT_BIN);

	result = DateTime(date.Year, date.Month, date.Date, time.Hours,
	                  time.Minutes, time.Seconds);

	return result;
}

void DateTimeManager::set(const DateTime& dateTime)
{
    RTC_TimeTypeDef time;

	time.Hours = dateTime.hour();
	time.Minutes = dateTime.minute();
	time.Seconds = dateTime.second();

	HAL_RTC_SetTime(&mHrtc, &time, RTC_FORMAT_BIN);

    RTC_DateTypeDef date; 

	date.Month = dateTime.month();
	date.Date = dateTime.day();
	date.Year = dateTime.numberYear();

	HAL_RTC_SetDate(&mHrtc, &date, RTC_FORMAT_BIN);
}


void DateTimeManager::setDate(const DateTime& dateTime)
{
	RTC_DateTypeDef date; 

	date.Month = dateTime.month();
	date.Date = dateTime.day();
	date.Year = dateTime.numberYear();

	HAL_RTC_SetDate(&mHrtc, &date, RTC_FORMAT_BIN);
}