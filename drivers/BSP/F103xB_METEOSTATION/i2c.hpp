#ifndef DRIVERS_BSP_F103XB_METEOSTATION_I2C
#define DRIVERS_BSP_F103XB_METEOSTATION_I2C

#include <drivers/bsp/ST/bsp_template.hpp>
#include <cinttypes>
#include <cstdint>

namespace bsp::i2c
{
using port_t = bsp::i2c::port;
constexpr std::uint32_t timeout = 100;

constexpr std::uint16_t ds3231_address = 0x68;
constexpr port_t ds3231_port = port_t::_1;

constexpr std::uint16_t bme280_address = 0x76;
constexpr port_t bme280_port = port_t::_1;

constexpr std::uint16_t lcd_address = 0x27;
constexpr port_t lcd_port = port_t::_1;

void init(port_t port);
bool send(port_t port, std::uint16_t address, uint8_t data[], std::size_t size);
bool receive(port_t port, std::uint16_t address, uint8_t data[], std::size_t size);
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_I2C */
