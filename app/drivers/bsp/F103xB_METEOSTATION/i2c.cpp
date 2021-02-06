#include <drivers/bsp/F103xB_METEOSTATION/i2c.hpp>
#include <i2c.h>

using namespace bsp::i2c;

namespace 
{
using i2c_handle_t = I2C_HandleTypeDef;

i2c_handle_t* get_hi2c(port_t port)
{
    using namespace bsp::i2c;

    switch(port) {
        case port_t::_1:
            return &hi2c1;
        case port_t::_2:
        default:
            return nullptr;
    }
}
}

void bsp::i2c::init(port_t port)
{
    if (port == port_t::_1) {
        MX_I2C1_Init();
    } 
}

bool bsp::i2c::send(port_t port, std::uint16_t address, uint8_t data[], std::size_t size)
{
    i2c_handle_t* hi2c = get_hi2c(port);

    if (hi2c == nullptr) return false;

    auto result = HAL_I2C_Master_Transmit(hi2c, address << 1, data, size, timeout);

    return result == HAL_OK;
}

bool bsp::i2c::receive(port_t port, std::uint16_t address, uint8_t data[], std::size_t size)
{
    i2c_handle_t* hi2c = get_hi2c(port);

    if (hi2c == nullptr) return false;

    auto result = HAL_I2C_Master_Receive(hi2c, address << 1, data, size, timeout);

    return result == HAL_OK;
}
