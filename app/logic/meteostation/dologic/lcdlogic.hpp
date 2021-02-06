#ifndef LOGIC_METEOSTATION_DOLOGIC_LCDLOGIC
#define LOGIC_METEOSTATION_DOLOGIC_LCDLOGIC

#include <lib/i2c/lcd/lcd.hpp>
#include <lib/task/taskbase.hpp>
#include <lib/datetime/datetime.hpp>
#include <lib/rotatebuffer.h>
#include <lib/stream/stringhandler.hpp>
#include <vector>

namespace applogic
{

class LcdLogic : public lib::task::TaskBase
{
public:
    using lcd_t = lib::i2c::lcd::Lcd;
    using datetime_t = lib::datetime::DateTime;


private:
    void draw_dig(uint8_t, uint8_t, uint8_t);
    void draw_dots(bool dotState, uint8_t x, uint8_t y);

    void load_clock();
    void draw_clock(uint8_t, uint8_t, uint8_t, uint8_t);
    void draw_date(const datetime_t&, uint8_t, uint8_t);
    void draw_co2(uint16_t, uint8_t, uint8_t);
    void draw_temp(float, uint8_t, uint8_t);
    void draw_humidity(uint8_t, uint8_t, uint8_t);
    void draw_pressure(uint16_t, uint8_t, uint8_t);
    void draw_rain_percent(int8_t, uint8_t, uint8_t);

    void main_show();

public:
    LcdLogic();
    virtual ~LcdLogic();

    void setup() override;
    void func() override;

private:
    lcd_t lcd;

private:
    inline static uint8_t LT[8] = {0b00111, 0b01111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};
    inline static uint8_t UB[8] = {0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};
    inline static uint8_t RT[8] = {0b11100, 0b11110, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};
    inline static uint8_t LL[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b01111, 0b00111};
    inline static uint8_t LB[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111};
    inline static uint8_t LR[8] = {0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11110, 0b11100};
    inline static uint8_t UMB[8] = {0b11111, 0b11111, 0b11111, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111};
    inline static uint8_t LMB[8] = {0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111};

    inline static std::vector<std::string> week_day = {
	    "Sun ",
	    "Mon ",
	    "Tue ",
	    "Wed ",
	    "Thu ",
	    "Fri ",
	    "Sat "
    };
};

}

#endif /* LOGIC_METEOSTATION_DOLOGIC_LCDLOGIC */
