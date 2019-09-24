#pragma once

#include <taskscheduler.h>
#include <lcd.h>
#include <i2cds18b20.h>
#include <ads7844.h>
#include <datetime.h>

class LcdTask : public TaskBase
{
private:
    static constexpr uint16_t lcd1Address = 0x3F;
    static constexpr uint16_t lcd2Address = 0x3B;

    Lcd lcd1;
    Lcd lcd2;

    Ds18b20 tempSource;
    Ads7844 analogInput;

    //static constexpr uint32_t dateFlashAddress = 0x100000;
    //void loadDate();
    //void saveDate(const DateTime&);

public:
    LcdTask();
    virtual ~LcdTask();

    void setup() override;
	void func() override;
};