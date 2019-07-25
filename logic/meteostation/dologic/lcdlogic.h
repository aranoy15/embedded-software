#pragma once

#include <inttypes.h>
#include <singleton.h>
#include <lcd.h>
#include <memory>
#include <timer.h>
#include <datetime.h>

class LcdLogic : public Singleton<LcdLogic>
{
private:
    static uint8_t LT[8]; 
    static uint8_t UB[8];
    static uint8_t RT[8];
    static uint8_t LL[8]; 
    static uint8_t LB[8]; 
    static uint8_t LR[8]; 
    static uint8_t UMB[8]; 
    static uint8_t LMB[8]; 

    static std::vector<std::string> weekDay;

private:
    std::unique_ptr<Lcd> lcd;
    std::unique_ptr<Timer> mTimer;
    bool mDotState;

private:
    void drawDig(uint8_t, uint8_t, uint8_t);
    void drawDots(bool dotState, uint8_t x, uint8_t y);

    void loadClock();
    void drawClock(uint8_t, uint8_t, uint8_t, uint8_t);
    void drawDate(const DateTime&, uint8_t, uint8_t);
    void drawCO2(uint16_t, uint8_t, uint8_t);
    void drawTemp(float, uint8_t, uint8_t);
    void drawHumidity(float, uint8_t, uint8_t);

public:
    LcdLogic();
    virtual ~LcdLogic();

    //void init();
    void processLcd();
};
