#ifndef DRIVERS_LIB_I2C_I2C
#define DRIVERS_LIB_I2C_I2C

#include <bsp.hpp>
#include <cstring>

namespace lib::i2c
{

using port_t = bsp::i2c::port_t;

template<port_t port>
struct I2C final
{
    static void init() noexcept
    {
        bsp::i2c::init(port);
    }

    static bool send_byte(std::uint16_t address, std::uint8_t data)
    {
        return send(address, &data, sizeof(data));
    }

    static std::uint8_t receive_byte(std::uint16_t address)
    {
        std::uint8_t result = 0;
        receive(address, &result, sizeof(result));
        return result;
    }

    static bool send(std::uint16_t address, std::uint8_t data[], std::size_t size)
    {
        return bsp::i2c::send(port, address, data, size);
    }

    static bool receive(std::uint16_t address, std::uint8_t data[], std::size_t size)
    {
        return bsp::i2c::receive(port, address, data, size);
    }

    template <typename TypeReg, typename TypeData>
	static bool send_reg(std::uint16_t address, TypeReg reg, TypeData data)
	{
		std::uint8_t sending_data[sizeof(TypeReg) + sizeof(TypeData)];

		std::memcpy(sending_data, reinterpret_cast<std::uint8_t*>(&reg), sizeof(TypeReg));
		std::memcpy(sending_data + sizeof(TypeReg), reinterpret_cast<uint8_t*>(&data),
		       sizeof(TypeData));

		return bsp::i2c::send(port, address, sending_data, sizeof(sending_data));
	}

	template <typename TypeResult, typename TypeReg>
	static TypeResult receive_reg(std::uint16_t address, TypeReg reg)
	{
		TypeResult resultData;
		receive_reg(address, reg, reinterpret_cast<uint8_t*>(&resultData), sizeof(TypeResult));
		return resultData;
	}

	template <typename TypeReg>
	static bool receive_reg(std::uint16_t address, TypeReg reg, std::uint8_t data[], std::size_t size)
	{
		if (not bsp::i2c::send(port, address, reinterpret_cast<uint8_t*>(&reg),
		                   sizeof(TypeReg)))
			return false;

		if (not bsp::i2c::receive(port, address, data, size))
			return false;

		return true;
	}
};
}

#endif /* DRIVERS_LIB_I2C_I2C */
