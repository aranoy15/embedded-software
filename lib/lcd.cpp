#include <lcd.h>
#include <time.h>

using namespace utils;

Lcd::Lcd(uint16_t address, uint8_t lcdCols, uint8_t lcdRows,
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

Lcd::~Lcd() {}

void Lcd::expanderWrite(uint8_t data)
{
    getI2c().writeByte(m_address, data | m_backLightVal);
}

void Lcd::pulseEnable(uint8_t data)
{
    expanderWrite(data | En);
    delayMicro(1);

    expanderWrite(data & ~En);
    delayMicro(50);
}

void Lcd::write4Bits(uint8_t value)
{
    expanderWrite(value);
    pulseEnable(value);
}

void Lcd::send(uint8_t value, uint8_t mode)
{
    uint8_t high = value & 0xf0;
    uint8_t low = (value << 4) & 0xf0;
    write4Bits((high) | mode);
    write4Bits((low) | mode);
}

void Lcd::command(uint8_t value)
{
    send(value, 0);
}

void Lcd::display()
{
    m_displayControl |= DISPLAYON;
    command(DISPLAYCONTROL | m_displayControl);
}

void Lcd::noDisplay()
{
    m_displayControl &= ~DISPLAYON;
    command(DISPLAYCONTROL | m_displayControl);
}

void Lcd::init()
{
	m_displayFunction = FOURBITMODE | ONELINE | FIVEx8DOTS;

	if (m_rows > 1)
        m_displayFunction |= TWOLINE;

    if (m_charSize != 0 and m_rows == 1) 
        m_displayFunction |= FIVEx10DOTS;

    Time::sleep(Time(50));

    expanderWrite(m_backLightVal);
    Time::sleep(Time::secs(1));

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

void Lcd::clear()
{
    command(CLEARDISPLAY);
    Time::sleep(Time(2));
}

void Lcd::home()
{
    command(RETURNHOME);
    Time::sleep(Time(2));
}

void Lcd::sendChar(char ch)
{
    send(ch, Rs);
}

void Lcd::sendString(std::string data)
{
    for (size_t i = 0; i < data.length(); i++) {
        sendChar(data[i]);
    }
}

void Lcd::setCursor(uint8_t col, uint8_t row)
{
	int rowOffsets[] = {0x00, 0x40, 0x14, 0x54};

    if (row > m_rows)
        row = m_rows - 1;

    command(SETDDRAMADDR | (col + rowOffsets[row]));
}

void Lcd::write(uint8_t value)
{
    send(value, Rs);
}

void Lcd::createChar(uint8_t location, uint8_t charmap[])
{
    location &= 0x7;
    command(SETCGRAMADDR | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}