#include <logic/meteostation/dologic/datastore.hpp>
#include <drivers/lib/uart/log/log.hpp>

using namespace applogic;

void DataStore::init() noexcept
{
    /*
    temp_hour.alloc(temp_hour_count);
    temp_day.alloc(temp_day_count);
    pressure_hour.alloc(pressure_hour_count);
    pressure_day.alloc(pressure_day_count);
    co2_hour.alloc(co2_hour_count);
    co2_day.alloc(co2_day_count);

    for (std::size_t i = 0; i < temp_hour_count; i++) {
        float value;
        bsp::flash::read(temp_hour_addr + (i * sizeof(value)), reinterpret_cast<uint8_t*>(&value), sizeof(value));
        temp_hour.put(value);
    }

    for (std::size_t i = 0; i < pressure_hour_count; i++) {
        uint16_t value;
        bsp::flash::read(pressure_hour_addr + (i * sizeof(value)), reinterpret_cast<uint8_t*>(&value), sizeof(value));
        pressure_hour.put(value);
    }

    for (std::size_t i = 0; i < co2_hour_count; i++) {
        uint16_t value;
        bsp::flash::read(co2_hour_addr + (i * sizeof(value)), reinterpret_cast<uint8_t*>(&value), sizeof(value));
        co2_hour.put(value);
    }

    for (std::size_t i = 0; i < temp_day_count; i++) {
        float value;
        bsp::flash::read(temp_day_addr + (i * sizeof(value)), reinterpret_cast<uint8_t*>(&value), sizeof(value));
        temp_day.put(value);
    }

    for (std::size_t i = 0; i < pressure_day_count; i++) {
        uint16_t value;
        bsp::flash::read(pressure_day_addr + (i * sizeof(value)), reinterpret_cast<uint8_t*>(&value), sizeof(value));
        pressure_day.put(value);
    }

    for (std::size_t i = 0; i < co2_day_count; i++) {
        uint16_t value;
        bsp::flash::read(co2_day_addr + (i * sizeof(value)), reinterpret_cast<uint8_t*>(&value), sizeof(value));
        co2_day.put(value);
    }

    _hour_timer.start(_hour_time);
    _day_timer.start(_day_time);
    */
}

void DataStore::update() noexcept
{
    /*
    static bool need_save = false;

    if (_hour_timer.elapsed()) {
        temp_hour.put(temperature);
        pressure_hour.put(pressure);
        co2_hour.put(co2);

        need_save = true;

        _hour_timer.start();
    }

    if (_day_timer.elapsed()) {
        temp_day.put(temp_hour.average());
        pressure_day.put(pressure_hour.average());
        co2_day.put(co2_hour.average());

        need_save = true;

        _day_timer.start();
    }

    if (need_save) {
        bsp::flash::erase();
        bsp::flash::write(temp_hour_addr, temp_hour.buffer(), temp_hour.buffer_size());
        bsp::flash::write(pressure_hour_addr, pressure_hour.buffer(), pressure_hour.buffer_size());
        bsp::flash::write(co2_hour_addr, co2_hour.buffer(), co2_hour.buffer_size());
        bsp::flash::write(temp_day_addr, temp_day.buffer(), temp_day.buffer_size());
        bsp::flash::write(pressure_day_addr, pressure_day.buffer(), pressure_day.buffer_size());
        bsp::flash::write(co2_day_addr, co2_day.buffer(), co2_day.buffer_size());
        need_save = false;
    }

    using namespace lib::uart::log;
    using namespace lib::stream;
    Log() << "Temp hour: " << temp_hour << Endl();
    Log() << "Pressure hour: " << pressure_hour << Endl();
    Log() << "Co2 hour: " << co2_hour << Endl();
    Log() << "Temp day: " << temp_day << Endl();
    Log() << "Pressure day: " << pressure_day << Endl();
    Log() << "Co2 day: " << co2_day << Endl();
    */
}
