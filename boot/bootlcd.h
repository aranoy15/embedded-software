#pragma once

#include <inttypes.h>
#include <bsp.h>

class BootLcd
{
private:
    enum Commands
    {
        CLEARDISPLAY = 0x01,
        RETURNHOME = 0x02,
        ENTRYMODESET = 0x04,
        DISPLAYCONTROL = 0x08,
        CURSORSHIFT = 0x10,
        FUNCTIONSET = 0x20,
        SETCGRAMADDR = 0x40,
        SETDDRAMADDR = 0x80
    };

    enum FlagsEntryMode
    {
        ENTRYRIGHT = 0x00,
        ENTRYLEFT = 0x02,
        ENTRYSHIFTINCREMENT = 0x01,
        ENTRYSHIFTDECREMENT = 0x00
    };

    enum FlagsOnOffControl
    {
        DISPLAYON = 0x04,
        DISPLAYOFF = 0x00,
        CURSORON = 0x02,
        CURSOROFF = 0x00,
        BLINKON = 0x01,
        BLINKOFF = 0x00
    };

	enum FlagsDisplayCursorShift
	{
		DISPLAYMOVE = 0x08,
		CURSORMOVE = 0x00,
		MOVERIGHT = 0x04,
		MOVELEFT = 0x00
	};

    enum FlagsFunctionSet
    {
        EIGHTBITMODE = 0x10,
        FOURBITMODE = 0x00,
        TWOLINE = 0x08,
        ONELINE = 0x00,
        FIVEx10DOTS = 0x04,
        FIVEx8DOTS = 0x00
    };

    enum FlagsBlacklightControl
    {
        BACKLIGHT = 0x08,
        NOBACKLIGHT = 0x00
    };

    enum
    {
        En = 1 << 2,
        Rw = 1 << 1,
        Rs = 1 << 0
    };

private:
    static I2C_HandleTypeDef m_hndl;

private:
    uint16_t m_address;
    uint8_t m_displayFunction;
    uint8_t m_displayControl;
    uint8_t m_displayMode;
    uint8_t m_cols;
    uint8_t m_rows;
    uint8_t m_charSize;
    uint8_t m_backLightVal;

private:
    static void sendByte(uint16_t address, uint8_t data);

public:
    BootLcd(uint16_t address, uint8_t lcdCols, uint8_t lcdRows, uint8_t charSize);
    virtual ~BootLcd();

    static void i2cInit();

    void init();

    void clear();
    void home();
    void sendChar(char);
    void sendString(char data[], uint16_t size);
    void setCursor(uint8_t col, uint8_t row);

    void command(uint8_t);
    void display();
    void noDisplay();
    void blink();
    void noBlink();
    void cursor();
    void noCursor();
    void createChar(uint8_t, uint8_t[]);
    void write(uint8_t);

private:
    void write4Bits(uint8_t);
    void expanderWrite(uint8_t);
    void send(uint8_t, uint8_t);
    void pulseEnable(uint8_t);
};