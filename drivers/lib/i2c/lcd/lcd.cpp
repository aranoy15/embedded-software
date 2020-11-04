#include <lib/i2c/lcd/lcd.hpp>
#include <lib/time/time.hpp>

using namespace lib::i2c::lcd;

Lcd::Lcd(uint8_t lcd_cols, uint8_t lcd_rows,
                 uint8_t char_size)
    : _display_function(0),
      _display_control(0),
      _display_mode(0),
      _cols(lcd_cols),
      _rows(lcd_rows),
      _char_size(char_size),
      _back_light_val(BACKLIGHT)
{
}

Lcd::~Lcd() {}

void Lcd::expander_write(uint8_t data)
{
    i2c_t::send_byte(_address, data | _back_light_val);
}

void Lcd::pulse_enable(uint8_t data)
{
    expander_write(data | En);
    //delayMicro(1);
    time::Time::sleep(time::Time(1));

    expander_write(data & ~En);
    //delayMicro(50);
    time::Time::sleep(time::Time(1));
}

void Lcd::write_4bits(uint8_t value)
{
    expander_write(value);
    pulse_enable(value);
}

void Lcd::send(uint8_t value, uint8_t mode)
{
    uint8_t high = value & 0xf0;
    uint8_t low = (value << 4) & 0xf0;
    write_4bits((high) | mode);
    write_4bits((low) | mode);
}

void Lcd::command(uint8_t value)
{
    send(value, 0);
}

void Lcd::display()
{
    _display_control |= DISPLAYON;
    command(DISPLAYCONTROL | _display_control);
}

void Lcd::no_display()
{
    _display_control &= ~DISPLAYON;
    command(DISPLAYCONTROL | _display_control);
}

void Lcd::init()
{
	_display_function = FOURBITMODE | ONELINE | FIVEx8DOTS;

	if (_rows > 1)
        _display_function |= TWOLINE;

    if (_char_size != 0 and _rows == 1) 
        _display_function |= FIVEx10DOTS;

    time::Time::sleep(time::Time(50));

    expander_write(_back_light_val);
    time::Time::sleep(time::Time::secs(1));

    write_4bits(0x03 << 4);
    //delay_micro(4500);
    time::Time::sleep(time::Time(5));

    write_4bits(0x03 << 4);
    //delayMicro(4500);
    time::Time::sleep(time::Time(5));

    write_4bits(0x03 << 4);
    //delayMicro(150);
    time::Time::sleep(time::Time(1));

    write_4bits(0x02 << 4);

    command(FUNCTIONSET | _display_function);

	_display_control = DISPLAYON | CURSOROFF | BLINKOFF;
	display();

    clear();

	_display_mode = ENTRYLEFT |
	                ENTRYSHIFTDECREMENT;
    command(ENTRYMODESET | _display_mode);

    home();
}

void Lcd::clear()
{
    command(CLEARDISPLAY);
    time::Time::sleep(time::Time(2));
}

void Lcd::home()
{
    command(RETURNHOME);
    time::Time::sleep(time::Time(2));
}

void Lcd::send_char(char ch)
{
    send(ch, Rs);
}

void Lcd::send_string(std::string data)
{
    for (size_t i = 0; i < data.length(); i++) {
        send_char(data[i]);
    }
}

void Lcd::set_cursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = {0x00, 0x40, 0x14, 0x54};

    if (row > _rows)
        row = _rows - 1;

    command(SETDDRAMADDR | (col + row_offsets[row]));
}

void Lcd::write(uint8_t value)
{
    send(value, Rs);
}

void Lcd::create_char(uint8_t location, uint8_t charmap[])
{
    location &= 0x7;
    command(SETCGRAMADDR | (location << 3));
    for (uint8_t i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}