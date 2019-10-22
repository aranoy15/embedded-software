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
    : TaskBase(3000, TaskPriority::Realtime), mTempSource(), mAnalogInput()
{
}

SensorTask::~SensorTask() {}

void SensorTask::setup()
{
}

void SensorTask::func()
{
    DataStorage& dataStorage = *DataStorage::instance();

    float boilingVoltage = Ads7844::calcValue(mAnalogInput.getValue(0));
    float condansatingVoltage = Ads7844::calcValue(mAnalogInput.getValue(1));

	//UsbDevice::instance()->send(utils::stringFormat("AI 1: %.2f", boilingVoltage));
	//UsbDevice::instance()->send(utils::stringFormat("AI 2: %.2f", condansatingVoltage));

    //dataStorage.mBoilingPressure = boilingVoltage * 0.09191 - 2.375 + 0.1;
    //dataStorage.mCondansatingPressure = condansatingVoltage * 0.36764 - 7.5 + 0.4;

    dataStorage.mBoilingPressure = utils::map(boilingVoltage, 0.4f, 2.0f, 0.0f, 10.0f);
    dataStorage.mCondansatingPressure = utils::map(condansatingVoltage, 0.4f, 2.0f, 0.0f, 30.0f);

    float lbp = std::log(dataStorage.mBoilingPressure + 1);
    float lcp = std::log(dataStorage.mCondansatingPressure + 1);

	dataStorage.mBoilingTemperature = ((0.3026 * lbp + 1.9907) * lbp + 20.4) * lbp - 36.891;
	dataStorage.mCondansatingTemperature = ((0.4183 * lcp + 1.666) * lcp + 21.165) * lcp - 43.994;

	dataStorage.mMinBoiling = std::min(dataStorage.mMinBoiling, dataStorage.mBoilingPressure);

	dataStorage.mMaxCondansating = std::max(dataStorage.mMaxCondansating, dataStorage.mCondansatingPressure);

    auto [resultEv, tempEv] = mTempSource.update(0);

	if (resultEv) {
		dataStorage.mEvaporatorTemperature = Ds18b20::calcValue(tempEv);
		dataStorage.mOverHeatingTemperature =
		    dataStorage.mEvaporatorTemperature -
		    dataStorage.mBoilingTemperature;
	} else
		dataStorage.mEvaporatorTemperature = -99.0f;

	auto [resultCon, tempCon] = mTempSource.update(1);

	if (resultCon) {
		dataStorage.mCondensatorTemperature = Ds18b20::calcValue(tempCon);
		dataStorage.mOverCoolingTemperature =
		    dataStorage.mCondansatingTemperature -
		    dataStorage.mCondensatorTemperature;
	} else
		dataStorage.mCondensatorTemperature = -99.0f;
}