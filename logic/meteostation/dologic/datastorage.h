#pragma once

#include <singleton.h>
#include <rotatebuffer.h>
#include <timer.h>

class DataStorage : public Singleton<DataStorage>
{
private:
	
	constexpr uint32_t hourTimeout() { return 4 * Time::minute(); }
	constexpr uint32_t dayTimeout() { return 1.6 * Time::hour(); }
	constexpr uint32_t infoTimeout() { return 1 * Time::second(); }

private:
    RotateBuffer<uint16_t> mCo2DayData;
    RotateBuffer<uint16_t> mCo2HourData;

    RotateBuffer<float> mTempDayData;
    RotateBuffer<float> mTempHourData;

    RotateBuffer<uint16_t> mHumidityDayData;
    RotateBuffer<uint16_t> mHumidityHourData;

    RotateBuffer<uint16_t> mPressureDayData;
    RotateBuffer<uint16_t> mPressureHourData;

    Timer mHourTimer;
    Timer mDayTimer;
    Timer mInfoTimer;

public:
    DataStorage();

    void processDataStorage();
};
