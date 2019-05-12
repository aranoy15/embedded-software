#pragma once

#include <singleton.h>

namespace i2c
{

enum I2CPorts 
{
    i2cPort1 = 1,
    i2cPort2 = 2
};

template<I2CPorts port>
class I2C : public Singleton<I2C<port> >
{
private:
	I2C_HandleTypeDef m_hndl;

private:
	I2C(const I2C&);
	I2C operator=(const I2C&);

public:
	I2C() : m_hndl(),
	{
	}

public:
	I2C_TypeDef* Port2CSP()
	{
		if (port == bsp::i2cPort1)
			return I2C1;
		else if (port == bsp::i2cPort2)
			return I2C2;

		return I2C3;
	}
};
}  // namespace i2c