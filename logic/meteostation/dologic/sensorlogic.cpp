#include <sensorlogic.h>

SensorLogic::SensorLogic()
    : mhz(new Mhz19()),
      bme(new Bme280(0x76)),
      mhzTimer(new Timer(5000)),
      bmeTimer(new Timer(1000)),
      mCo2(0),
      mTemp(0),
      mHumidity(0)
{
}

void SensorLogic::co2Init()
{
    mhz->init();
    mhz->setAutoCalibration(false);
}

void SensorLogic::tempInit()
{
    bme->init();
}

SensorLogic::~SensorLogic() {}

void SensorLogic::processSensor()
{
    if (mhzTimer->elapsed()) {
        mCo2 = mhz->getCO2();
        //mTemp = bme->readTemperature();

        mhzTimer->start();
    }

    if (bmeTimer->elapsed()) {
        mTemp = bme->readTemperature();
        mHumidity = bme->readHumidity();

        bmeTimer->start();
    }
}