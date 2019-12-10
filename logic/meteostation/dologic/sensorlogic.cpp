#include <sensorlogic.h>

SensorLogic::SensorLogic()
    : mhz(new Mhz19()),
      bme(new Bme280(0x76)),
      mhzTimer(new Timer(5000)),
      bmeTimer(new Timer(1000)),
      pressureRainTimer(10 * Time::minute()),
      mCo2(0),
      mTemp(0),
      mHumidity(0),
      mPressure(0),
      mPressureArray(),
      mRainPercent(0)
{
    mPressureArray.alloc(5);
}

void SensorLogic::co2Init()
{
    mhz->init();
    mhz->setAutoCalibration(false);
    //mhz->setRange(Mhz19Range::Range5000);
}

void SensorLogic::tempInit()
{
    bme->init();

	bme->setSampling(Bme280::MODE_FORCED,
	                 Bme280::SAMPLING_X1,  // temperature
	                 Bme280::SAMPLING_X1,  // pressure
	                 Bme280::SAMPLING_X1,  // humidity
	                 Bme280::FILTER_OFF);

    bme->takeForcedMeasurement();
    float data = bme->readPressure();

    for (uint8_t i = 0; i < 5; i++)
        mPressureArray.put(data);
}

SensorLogic::~SensorLogic() {}

void SensorLogic::updateBme()
{
	bme->takeForcedMeasurement();

	mTemp = bme->readTemperature();
	mHumidity = static_cast<uint8_t>(bme->readHumidity());
	mPressure = static_cast<uint16_t>(bme->readPressure() / 133.3f);
}

void SensorLogic::updateCo2()
{
    mCo2 = mhz->getCO2();
}

void SensorLogic::processSensor()
{
    if (mhzTimer->elapsed()) {
        updateCo2();
        mhzTimer->start();
    }

    if (bmeTimer->elapsed()) {
        updateBme();
        bmeTimer->start();
    }

    if (pressureRainTimer.elapsed()) {
		long averPress = 0;
		for (uint8_t i = 0; i < 10; i++) {
			bme->takeForcedMeasurement();
			averPress += bme->readPressure();
            Time::sleep(Time(1));
		}
		averPress /= 10;

        mPressureArray.put(averPress);

		uint32_t sumX = 0, sumY = 0, sumX2 = 0, sumXY = 0;

        for (uint8_t i = 0; i < 6; i++) {
			sumX += i;
			sumY += (long)mPressureArray.vector()[i];
			sumX2 += i * i;
			sumXY += (long)i * mPressureArray.vector()[i];
		}

		float a = 0;
		a = (long)6 * sumXY;  // расчёт коэффициента наклона приямой
		a = a - (long)sumX * sumY;
		a = (float)a / (6 * sumX2 - sumX * sumX);

        int delta = a * 6;      // расчёт изменения давления

        mRainPercent = map(delta, -250, 250, 100, -100);

		pressureRainTimer.start();
    }
}