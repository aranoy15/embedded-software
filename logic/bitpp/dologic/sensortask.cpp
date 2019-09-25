#include <sensortask.h>
#include <datastorage.h>
#include <cmath>
#include <algorithm>

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

    float input0 = Ads7844::calcValue(mAnalogInput.getValue(0));
    float input1 = Ads7844::calcValue(mAnalogInput.getValue(0));

    dataStorage.mEightPressure = input0 * 0.09191 - 2.375 + 0.1;
    dataStorage.mThirtyPressure = input1 * 0.36764 - 7.5 + 0.4;

    float lep = log(dataStorage.mEightPressure + 1);
    float ltp = log(dataStorage.mThirtyPressure + 1);

	dataStorage.mBoilingTemperature =
	    0.3451 * pow(lep, 3) + 2.1642 * pow(lep, 2) + 21.727 * lep + 26.307;

	dataStorage.mCondansatingTemperature =
	    0.3451 * pow(ltp, 3) +
	    2.1642 * pow(ltp, 2) +
	    21.727 * ltp - 26.307;

	dataStorage.mMinBoiling =
	    std::min(dataStorage.mMinBoiling, dataStorage.mEightPressure);

	dataStorage.mMaxCondansating =
	    std::max(dataStorage.mMaxCondansating, dataStorage.mThirtyPressure);

    auto [result, temp] = mTempSource.update(0);

    if (result) dataStorage.mEvaporatorTemperature = temp;
    else dataStorage.mEvaporatorTemperature = -99.0f;

    auto [result, temp] = mTempSource.update(1);

    if (result) dataStorage.mCondensatorTemperature = temp;
    else dataStorage.mCondensatorTemperature = -99.0f;

	dataStorage.overHeatingTemp =
	    dataStorage.mEvaporatorTemperature - dataStorage.mBoilingTemperature;

	dataStorage.overCoolingTemp = dataStorage.mCondansatingTemperature -
	                              dataStorage.mCondensatorTemperature;

}