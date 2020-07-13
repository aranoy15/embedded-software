#include <sensortask.h>
#include <datastorage.h>
#include <cmath>
#include <algorithm>
#include <usb.h>

namespace
{
__attribute__((constructor)) void reg()
{
	TaskScheduler::instance()->reg(new SensorTask());
}
}  // namespace

SensorTask::SensorTask()
    : TaskBase(Time::second(), TaskPriority::Realtime),
      mTempSource(),
      mAnalogInput(),
      _evaporator_sensor(0),
	  _condensator_sensor(1),
	  _current_sensor(0),
	  _switch_timer(2000)
{
}

SensorTask::~SensorTask() {}

void SensorTask::setup()
{
}

void SensorTask::func()
{
    DataStorage& dataStorage = *DataStorage::instance();

	uint8_t boilingAddress = 0;
	uint8_t condansatingAddress = 1;

    float boilingVoltage = Ads7844::calcValue(mAnalogInput.getValue(boilingAddress));
    float condansatingVoltage = Ads7844::calcValue(mAnalogInput.getValue(condansatingAddress));

    dataStorage.mBoilingPressure = utils::map(boilingVoltage, 0.4f, 2.0f, 0.0f, 10.0f);
    dataStorage.mCondansatingPressure = utils::map(condansatingVoltage, 0.4f, 2.0f, 0.0f, 30.0f);

	if (dataStorage.mBoilingPressure < 0.0f) dataStorage.mBoilingPressure = 0.0f;
	if (dataStorage.mBoilingPressure > 10.0f) dataStorage.mBoilingPressure = 10.0f;

	if (dataStorage.mCondansatingPressure < 0.0f) dataStorage.mCondansatingPressure = 0.0f;
	if (dataStorage.mCondansatingPressure > 30.0f) dataStorage.mCondansatingPressure = 30.0f;

    float lbp = std::log(dataStorage.mBoilingPressure + 1);
    float lcp = std::log(dataStorage.mCondansatingPressure + 1);

#if (FRION_TYPE == 0)
#warning Calc frion 407c
	dataStorage.mBoilingTemperature = ((0.3026 * lbp + 1.9907) * lbp + 20.4) * lbp - 36.891;
	dataStorage.mCondansatingTemperature = ((0.4183 * lcp + 1.666) * lcp + 21.165) * lcp - 43.994;
#elif (FRION_TYPE == 1)
#warning Calc frion 134a
	dataStorage.mBoilingTemperature = 0.3451 * std::pow(lbp, 3) + 2.1642 * std::pow(lbp, 2) + 21.727 * lbp - 26.307;
	dataStorage.mCondansatingTemperature = 0.3451 * std::pow(lcp, 3) + 2.1642 * std::pow(lcp, 2) + 21.727 * lcp - 26.307;
#endif

	dataStorage.mMinBoiling = std::min(dataStorage.mMinBoiling, dataStorage.mBoilingPressure);
	dataStorage.mMaxCondansating = std::max(dataStorage.mMaxCondansating, dataStorage.mCondansatingPressure);

	_evaporator_sensor.update();
	dataStorage.mEvaporatorTemperature = _evaporator_sensor.value();
	dataStorage.mOverHeatingTemperature = dataStorage.mEvaporatorTemperature - dataStorage.mBoilingTemperature;

	_condensator_sensor.update();
	dataStorage.mCondensatorTemperature = _condensator_sensor.value();
	dataStorage.mOverCoolingTemperature = dataStorage.mCondansatingTemperature - dataStorage.mCondensatorTemperature;

	/*
	switch (_current_sensor) {
		case 0:
			if (not _switch_timer.is_started()) _switch_timer.start();
			if (_switch_timer.elapsed()) {
				_current_sensor = 1;
				_switch_timer.stop();
			}

			if (_evaporator_sensor.update()) {
				dataStorage.mEvaporatorTemperature = _evaporator_sensor.value<float>();
				dataStorage.mOverHeatingTemperature = dataStorage.mEvaporatorTemperature - dataStorage.mBoilingTemperature;
				_current_sensor = 1;
				_switch_timer.stop();
			}
			break;

		case 1:
			if (not _switch_timer.is_started()) _switch_timer.start();
			if (_switch_timer.elapsed()) {
				_current_sensor = 0;
				_switch_timer.stop();
			}

			if (_condensator_sensor.update()) {
				dataStorage.mCondensatorTemperature = _condensator_sensor.value<float>();
				dataStorage.mOverCoolingTemperature = dataStorage.mCondansatingTemperature - dataStorage.mCondensatorTemperature;
				_current_sensor = 0;
				_switch_timer.stop();
			}
			break;
	}
	*/

	/*
    auto [resultEv, tempEv] = mTempSource.update(0);

	if (resultEv) {
		dataStorage.mEvaporatorTemperature = Ds18b20::calcValue(tempEv);
		dataStorage.mOverHeatingTemperature =
		    dataStorage.mEvaporatorTemperature -
		    dataStorage.mBoilingTemperature;
	} else
		dataStorage.mEvaporatorTemperature = -99.0f;
	*/

	/*
	auto [resultCon, tempCon] = mTempSource.update(1);

	if (resultCon) {
		dataStorage.mCondensatorTemperature = Ds18b20::calcValue(tempCon);
		dataStorage.mOverCoolingTemperature =
		    dataStorage.mCondansatingTemperature -
		    dataStorage.mCondensatorTemperature;
	} else
		dataStorage.mCondensatorTemperature = -99.0f;
	*/
}