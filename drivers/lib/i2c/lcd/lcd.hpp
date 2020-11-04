#pragma once

#include <lib/i2c/i2c.hpp>
#include <string>
#include <bsp.hpp>

namespace lib::i2c::lcd
{

class Lcd
{

public:
    Lcd(uint8_t lcd_cols, uint8_t lcd_rows, uint8_t char_size);
    virtual ~Lcd();

    void init();

    void clear();
    void home();
    void send_char(char ch);
    void send_string(std::string data);
    void set_cursor(uint8_t col, uint8_t row);

    void command(uint8_t data);
    void display();
    void no_display();
    void blink();
    void no_blink();
    void cursor();
    void no_cursor();
    void create_char(uint8_t, uint8_t[]);
    void write(uint8_t);

private:
    using i2c_t = I2C<bsp::i2c::lcd_port>;

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
    uint16_t _address = bsp::i2c::lcd_address;
    uint8_t _display_function;
    uint8_t _display_control;
    uint8_t _display_mode;
    uint8_t _cols;
    uint8_t _rows;
    uint8_t _char_size;
    uint8_t _back_light_val;

private:
    void write_4bits(uint8_t);
    void expander_write(uint8_t);
    void send(uint8_t, uint8_t);
    void pulse_enable(uint8_t);
};

}
