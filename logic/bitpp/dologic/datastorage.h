#pragma once

#include <singleton.h>

class SensorTask;

class DataStorage : public Singleton<DataStorage>
{
    friend class SensorTask;

    float mEvaporatorTemperature;
    float mCondensatorTemperature;

    float mBoilingPressure;
    float mCondansatingPressure;

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
	      mBoilingPressure(0),
	      mCondansatingPressure(0),
		  mBoilingTemperature(0),
		  mCondansatingTemperature(0),
		  mMinBoiling(10.0f),
		  mMaxCondansating(0.0f),
		  mOverHeatingTemperature(0),
		  mOverCoolingTemperature(0)
	{
	}

	float evaporatorTemp() const { return mEvaporatorTemperature; }
	float condensatorTemp() const { return mCondensatorTemperature; }

	float boilingPressure() const { return mBoilingPressure; }
	float condansatingPressure() const { return mCondansatingPressure; }

	float boilingTemp() const { return mBoilingTemperature; }
	float condansationTemp() const { return mCondansatingTemperature; }

	float minBoiling() const { return mMinBoiling; }
	float maxCondansating() const { return mMaxCondansating; }

	float overHeatingTemp() const { return mOverHeatingTemperature; }
	float overCoolingTemp() const { return mOverCoolingTemperature; }
};