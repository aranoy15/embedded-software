#include <bsp.hpp>
#include <logic/meteostation/dologic/sensorlogic.hpp>
#include <drivers/lib/time/timer.hpp>
#include <drivers/lib/uart/uart.hpp>
#include <drivers/lib/uart/log/log.hpp>
#include <drivers/lib/i2c/i2c.hpp>
#include <logic/meteostation/dologic/datastore.hpp>

using namespace applogic;
using namespace lib::time;

SensorLogic::SensorLogic()
    : lib::task::TaskBase(Time::secs(1), lib::task::Priority::High),
      mhz(),
      bme(),
      dt()
{
}

SensorLogic::~SensorLogic() {}

void SensorLogic::setup()
{
    lib::i2c::I2C<bsp::i2c::bme280_port>::init();
    lib::uart::Uart<bsp::usart::mhz_port>::init(false);

    bool result_init = bme.init();

    if (not result_init) bsp::reset();

	bme.set_sampling(bme_t::MODE_FORCED,
	                 bme_t::SAMPLING_X1,  // temperature
	                 bme_t::SAMPLING_X1,  // pressure
	                 bme_t::SAMPLING_X1,  // humidity
	                 bme_t::FILTER_OFF);

    mhz.init();
    mhz.set_auto_calibration(false);

    DataStore::init();
}

void SensorLogic::func()
{
    using namespace lib::uart::log;
    using namespace lib::datetime;

    DateTime current = dt.now();
    std::uint16_t co2 = mhz.get_co2();

    bme.take_forced_measurement();
    float temp = bme.read_temperature();
    float pressure = bme.read_pressure() / 133.3f;
    float humidity = bme.read_humidity();

    DataStore::co2 = co2;
    DataStore::pressure = pressure;
    DataStore::datetime = current;

    if (humidity != bme_t::error_value)
        DataStore::humidity = humidity;

    if (temp != bme_t::error_value)
        DataStore::temperature = temp;

    Log() << "[" << current << "] ";
    Log() << co2 << "ppm ";
    Log() << temp << "*C ";
    Log() << pressure << "bar ";
    Log() << humidity << "%" << lib::stream::Endl();

    std::string data;
    lib::stream::Stream::ResultRead result;

    Log() >> data >> result;

	if (result == lib::stream::Stream::ResultRead::OK) {
		if (data.rfind("AT+SETDATETIME ", 0) == 0) {
			data.erase(0, 15);
			DateTime receive_dt(data.c_str());
			dt.adjust(receive_dt);
			Log() << "AT+SETDATETIME:OK" << lib::stream::Endl();
		}
	}

    DataStore::update();
}


/*
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
*/
