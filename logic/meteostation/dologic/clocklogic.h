#pragma once

#include <singleton.h>
#include <memory>
#include <ds3231.h>
#include <timer.h>

class ClockLogic : public Singleton<ClockLogic>
{
private:
    std::unique_ptr<Ds3231> ds;
    std::unique_ptr<Timer> mTimer;

    DateTime mCurrentDateTime;

public:
    ClockLogic();
    virtual ~ClockLogic();

    void processClock();

	const DateTime& getDateTime() const { return mCurrentDateTime; }
};