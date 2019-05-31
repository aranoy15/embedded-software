#pragma once

#include <thread.h>
#include <gpio.h>
#include <time.h>

class BlinkThread: public Thread
{
public:
	BlinkThread();
	virtual ~BlinkThread() {}

    void threadFunc() override;
};