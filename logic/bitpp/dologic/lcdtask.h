#pragma once

#include <taskscheduler.h>
#include <lcd.h>
#include <datetime.h>

class LcdTask : public TaskBase
{
private:
    static constexpr uint16_t lcd1Address = 0x3F;
    static constexpr uint16_t lcd2Address = 0x3B;

    Lcd lcd1;
    Lcd lcd2;

public:
    LcdTask();
    virtual ~LcdTask();

    void setup() override;
	void func() override;
};