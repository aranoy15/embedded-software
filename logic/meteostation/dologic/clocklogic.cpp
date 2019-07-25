#include <clocklogic.h>

ClockLogic::ClockLogic()
    : ds(new Ds3231(0x68)), mTimer(new Timer(1000)), mCurrentDateTime()
{
    /* 
    mCurrentDateTime = DateTime(__DATE__, __TIME__);
    ds->adjust(mCurrentDateTime);
    */
}

ClockLogic::~ClockLogic() {}

void ClockLogic::processClock()
{
    if (mTimer->elapsed()) {
        mCurrentDateTime = ds->now();
        mTimer->start();
    }
}