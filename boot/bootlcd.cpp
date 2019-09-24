#include <bootlcd.h>
#include <sleep.h>


I2C_HandleTypeDef BootLcd::m_hndl;


void BootLcd::i2cInit()
{
    m_hndl.Instance = bsp::i2c::port2CSP(bsp::lcd::lcdI2CPort);
    m_hndl.Init.ClockSpeed = 100000;
    m_hndl.Init.DutyCycle = I2C_DUTYCYCLE_2;
    m_hndl.Init.OwnAddress1 = 0;
    m_hndl.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    m_hndl.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    m_hndl.Init.OwnAddress2 = 0;
    m_hndl.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    m_hndl.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    bsp::i2c::init(bsp::lcd::lcdI2CPort);
    HAL_I2C_Init(&m_hndl);
}

void BootLcd::sendByte(uint16_t address, uint8_t data)
{
    HAL_I2C_Master_Transmit(&m_hndl, address << 1, &data, 1, 100);
}

static void delayMicro(uint32_t micros)
{
	micros *= (SystemCoreClock / 1000000) / 9;
	while (micros--);
}

BootLcd::BootLcd(uint16_t address, uint8_t lcdCols, uint8_t lcdRows,
                 uint8_t charSize)
    : m_address(address),
      m_displayFunction(0),
      m_displayControl(0),
      m_displayMode(0),
      m_cols(lcdCols),
      m_rows(lcdRows),
      m_charSize(charSize),
      m_backLightVal(BACKLIGHT)
{
	init();
}

BootLcd::~BootLcd() {}

void BootLcd::expanderWrite(uint8_t data)
{
    //getI2c().writeByte(m_address, data | m_backLightVal);
    sendByte(m_address, data);
}

void BootLcd::pulseEnable(uint8_t data)
{
    expanderWrite(data | En);
    delayMicro(1);

    expanderWrite(data & ~En);
    delayMicro(50);
}

void BootLcd::write4Bits(uint8_t value)
{
    expanderWrite(value);
    pulseEnable(value);
}

void BootLcd::send(uint8_t value, uint8_t mode)
{
    uint8_t high = value & 0xf0;
    uint8_t low = (value << 4) & 0xf0;
    write4Bits((high) | mode);
    write4Bits((low) | mode);
}

void BootLcd::command(uint8_t value)
{
    send(value, 0);
}

void BootLcd::display()
{
    m_displayControl |= DISPLAYON;
    command(DISPLAYCONTROL | m_displayControl);
}

void BootLcd::noDisplay()
{
    m_displayControl &= ~DISPLAYON;
    command(DISPLAYCONTROL | m_displayControl);
}

void BootLcd::init()
{
	m_displayFunction = FOURBITMODE | ONELINE | FIVEx8DOTS;

	if (m_rows > 1)
        m_displayFunction |= TWOLINE;

    if (m_charSize != 0 and m_rows == 1) 
        m_displayFunction |= FIVEx10DOTS;

    //Time::sleep(Time(50));
    sleep(50);

    expanderWrite(m_backLightVal);
    //Time::sleep(Time::secs(1));
    sleep(1000);

    write4Bits(0x03 << 4);
    delayMicro(4500);

    write4Bits(0x03 << 4);
    delayMicro(4500);

    write4Bits(0x03 << 4);
    delayMicro(150);

    write4Bits(0x02 << 4);

    command(FUNCTIONSET | m_displayFunction);

	m_displayControl = DISPLAYON | CURSOROFF | BLINKOFF;
	display();

    clear();

	m_displayMode = ENTRYLEFT |
	                ENTRYSHIFTDECREMENT;
    command(ENTRYMODESET | m_displayMode);

    home();
}

void BootLcd::clear()
{
    command(CLEARDISPLAY);
    //Time::sleep(Time(2));
    sleep(2);
}

void BootLcd::home()
{
    command(RETURNHOME);
    //Time::sleep(Time(2));
    sleep(2);
}

void BootLcd::sendChar(char ch)
{
    send(ch, Rs);
}

void BootLcd::sendString(char data[], uint16_t size)
{
    for (uint16_t i = 0; i < size; i++) {
        sendChar(data[i]);
    }
}

void BootLcd::setCursor(uint8_t col, uint8_t row)
{
	int rowOffsets[] = {0x00, 0x40, 0x14, 0x54};

    if (row > m_rows)
        row = m_rows - 1;

    command(SETDDRAMADDR | (col + rowOffsets[row]));
}

void BootLcd::write(uint8_t value)
{
    send(value, Rs);
}

void BootLcd::createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7;
    command(SETCGRAMADDR | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}