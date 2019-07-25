#pragma once

#include <singleton.h>
#include <mhz19.h>
#include <bme280.h>
#include <memory>

class SensorLogic : public Singleton<SensorLogic>
{
private:
    std::unique_ptr<Mhz19> mhz;
    std::unique_ptr<Bme280> bme;

    std::unique_ptr<Timer> mhzTimer;
    std::unique_ptr<Timer> bmeTimer;

    uint16_t mCo2;
    float mTemp;
    float mHumidity;

public:

    SensorLogic();
    virtual ~SensorLogic();

    void processSensor();

	uint16_t getCO2() const { return mCo2; }
	float getTemp() const { return mTemp; }
	float getHumidity() const { return mHumidity; }

    void co2Init();
    void tempInit();
};