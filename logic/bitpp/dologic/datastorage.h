#pragma once

#include <singleton.h>

class SensorTask;

class DataStorage : public Singleton<DataStorage>
{
    friend class SensorTask;

    float mEvaporatorTemperature;
    float mCondensatorTemperature;

    float mEightPressure;
    float mThirtyPressure;

	float mBoilingTemperature;
	float mCondansatingTemperature;

	float mMinBoiling;
	float mMaxCondansating;

	float mOverHeatingTemperature;
	float mOverCoolingTemperature;

public:

    DataStorage(const DataStorage&) = delete;
    DataStorage operator=(const DataStorage&) = delete;

	DataStorage()
	    : mEvaporatorTemperature(0),
	      mCondensatorTemperature(0),
	      mEightPressure(0),
	      mThirtyPressure(0)
	{
	}

	float evaporatorTemp() const { return mEvaporatorTemperature; }
	float condensatorTemp() const { return mCondensatorTemperature; }

	float eightPressure() const { return mEightPressure; }
	float thirtyPressure() const { return mThirtyPressure; }

	float boilingTemp() const { return mBoilingTemperature; }
	float condansationTemp() const { return mCondensatorTemperature; }

	float minBoiling() const { return mMinBoiling; }
	float maxCondansating() const { return mMaxCondansating; }

	float overHeatingTemp() const { return mOverHeatingTemperature; }
	float overCoolingTemp() const { return mOverCoolingTemperature; }
};