#ifndef LOGIC_METEOSTATION_DOLOGIC_DATASTORE
#define LOGIC_METEOSTATION_DOLOGIC_DATASTORE

#include <cinttypes>
#include <drivers/lib/datetime/datetime.hpp>
#include <drivers/lib/rotatebuffer.h>
#include <drivers/lib/time/timer.hpp>
#include <bsp.hpp>

namespace applogic
{
struct DataStore final
{
    DataStore() = delete;

    inline static std::uint16_t co2 = 0;
    inline static float temperature;
    inline static float humidity = 0;
    inline static float pressure;
    inline static lib::datetime::DateTime datetime;

    /*
    inline static lib::RotateBuffer<float> temp_hour;
    inline static lib::RotateBuffer<float> temp_day;
    inline static lib::RotateBuffer<uint16_t> pressure_hour;
    inline static lib::RotateBuffer<uint16_t> pressure_day;
    inline static lib::RotateBuffer<uint16_t> co2_hour;
    inline static lib::RotateBuffer<uint16_t> co2_day;
    */

    static void init() noexcept;
    static void update() noexcept;

private:
    /*
    static constexpr std::size_t temp_hour_count = 15;
    static constexpr std::size_t pressure_hour_count = 15;
    static constexpr std::size_t co2_hour_count = 15;

    static constexpr std::size_t temp_day_count = 16;
    static constexpr std::size_t pressure_day_count = 16;
    static constexpr std::size_t co2_day_count = 16;

    static constexpr std::size_t temp_hour_size = temp_hour_count * sizeof(float);
    static constexpr std::size_t pressure_hour_size = pressure_hour_count * sizeof(std::uint16_t);
    static constexpr std::size_t co2_hour_size = co2_hour_count * sizeof(std::uint16_t);

    static constexpr std::size_t temp_day_size = temp_day_count * sizeof(float);
    static constexpr std::size_t pressure_day_size = pressure_day_count * sizeof(std::uint16_t);
    static constexpr std::size_t co2_day_size = co2_day_count * sizeof(std::uint16_t);

	static constexpr std::size_t total_size =
	    temp_hour_size + pressure_hour_size + co2_hour_size + temp_day_size +
	    pressure_day_size + co2_day_size;

	static constexpr std::uint32_t temp_hour_addr = bsp::flash::start_address;
    static constexpr std::uint32_t temp_day_addr = temp_hour_addr + temp_hour_size;
    static constexpr std::uint32_t pressure_hour_addr = temp_day_addr + temp_day_size;
    static constexpr std::uint32_t pressure_day_addr = pressure_hour_addr + pressure_hour_size;
    static constexpr std::uint32_t co2_hour_addr = pressure_day_addr + pressure_day_size;
    static constexpr std::uint32_t co2_day_addr = co2_hour_addr + co2_hour_size;

    inline static lib::time::Timer _hour_timer;
    inline static lib::time::Timer _day_timer;
    inline static const lib::time::Time _hour_time = lib::time::Time::secs(1);
    inline static const lib::time::Time _day_time = lib::time::Time::secs(15);
    */
};
}

#endif /* LOGIC_METEOSTATION_DOLOGIC_DATASTORE */
