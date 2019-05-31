#pragma once

#include <thread.h>
#include <cmsis_os.h>
#include <gpio.h>
#include <time.h>
#include <lcd.h>

class MainThread : public Thread
{
public:
    MainThread();
    virtual ~MainThread();

    void threadFunc() override;
};