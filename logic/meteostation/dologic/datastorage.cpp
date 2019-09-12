#include <datastorage.h>
#include <uart.h>
#include <sensorlogic.h>

DataStorage::DataStorage()
    : mCo2DayData(),
      mCo2HourData(),
      mTempDayData(),
      mTempHourData(),
      mHumidityDayData(),
      mHumidityHourData(),
      mPressureDayData(),
      mPressureHourData(),
      mHourTimer(hourTimeout()),
      mDayTimer(dayTimeout()),
      mInfoTimer(infoTimeout())
{
    mCo2DayData.alloc(15);
    mCo2HourData.alloc(15);
    mTempDayData.alloc(15);
    mTempHourData.alloc(15);
    mHumidityDayData.alloc(15);
    mHumidityHourData.alloc(15);
    mPressureDayData.alloc(15);
    mPressureHourData.alloc(15);
}

void DataStorage::processDataStorage()
{
    if (mHourTimer.elapsed()) {
        mCo2HourData.put(SensorLogic::instance()->getCO2());
        mTempHourData.put(SensorLogic::instance()->getTemp());
        mPressureHourData.put(SensorLogic::instance()->getPressure());
        mHumidityHourData.put(SensorLogic::instance()->getHumidity());

        mHourTimer.start();
    }

    if (mDayTimer.elapsed()) {
        mCo2DayData.put(mCo2HourData.median());
        mTempDayData.put(mTempHourData.median());
        mPressureDayData.put(mPressureHourData.median());

        mDayTimer.start();
    }

    if (mInfoTimer.elapsed()) {
        std::string res; 
        Uart<bsp::uartP1>& log = *Uart<bsp::uartP1>::instance();

        res = "Co2 hour: ";

        for (auto item : mCo2HourData.vector()) {
            res.append(utils::stringFormat("%u ", item));
        }

        log.send(res);

        res = "Co2 day: ";

        for (auto item : mCo2DayData.vector()) {
            res.append(utils::stringFormat("%u ", item));
        }

        log.send(res);

        res = "Temp hour: ";

        for (auto item : mTempHourData.vector()) {
            res.append(utils::stringFormat("%s ", utils::ftostring(item).c_str()));
        }

        log.send(res);

        res = "Temp day: ";

        for (auto item : mTempDayData.vector()) {
            res.append(utils::stringFormat("%s ", utils::ftostring(item).c_str()));
        }

        log.send(res);

        res = "Pressure hour: ";

        for (auto item : mPressureHourData.vector()) {
            res.append(utils::stringFormat("%u ", item));
        }

        log.send(res);

        res = "Pressure day: ";

        for (auto item : mPressureDayData.vector()) {
            res.append(utils::stringFormat("%u ", item));
        }

        log.send(res);
        log.send(" ");

        mInfoTimer.start();
    }
}