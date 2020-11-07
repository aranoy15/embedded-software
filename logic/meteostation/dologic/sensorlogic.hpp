#ifndef LOGIC_METEOSTATION_DOLOGIC_SENSORLOGIC
#define LOGIC_METEOSTATION_DOLOGIC_SENSORLOGIC

#include <drivers/lib/uart/sensors/mhz19.hpp>
#include <drivers/lib/i2c/sensors/bme280.hpp>
#include <drivers/lib/task/taskbase.hpp>
#include <drivers/lib/i2c/datetime/ds3231.hpp>

namespace applogic
{

class SensorLogic : public lib::task::TaskBase
{
public:
    using mhz_t = lib::uart::sensors::Mhz19;
    using bme_t = lib::i2c::sensors::Bme280;
    using datetime_t = lib::i2c::datetime::Ds3231;

public:
    SensorLogic();
    virtual ~SensorLogic();

    void setup() override;
    void func() override;

private:
    mhz_t mhz;
    bme_t bme;
    datetime_t dt;

private:
    long map(long x, long in_min, long in_max, long out_min, long out_max)
    {
	    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};

}

#endif /* LOGIC_METEOSTATION_DOLOGIC_SENSORLOGIC */

