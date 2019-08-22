#pragma once

#include <singleton.h>
#include <mhz19.h>
#include <bme280.h>
#include <memory>
#include <rotatebuffer.h>

class SensorLogic : public Singleton<SensorLogic>
{
private:
    std::unique_ptr<Mhz19> mhz;
    std::unique_ptr<Bme280> bme;

    std::unique_ptr<Timer> mhzTimer;
    std::unique_ptr<Timer> bmeTimer;
    Timer pressureRainTimer;

    uint16_t mCo2;
    float mTemp;
    uint8_t mHumidity;
    uint16_t mPressure;

    int8_t mRainPercent;

    RotateBuffer<float> mPressureArray;

private:
    long map(long x, long in_min, long in_max, long out_min, long out_max)
    {
	    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

public:

    SensorLogic();
    virtual ~SensorLogic();

    void processSensor();

	uint16_t getCO2() const { return mCo2; }
	float getTemp() const { return mTemp; }
	uint8_t getHumidity() const { return mHumidity; }
	uint16_t getPressure() const { return mPressure; }
	int8_t getRainPercent() const { return mRainPercent; }

	void updateBme();
    void updateCo2();

	void co2Init();
    void tempInit();
};