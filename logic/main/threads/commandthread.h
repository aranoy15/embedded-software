#pragma once

#include <thread.h>
#include <uart.h>
#include <time.h>

class CommandThread : public Thread
{
private:
    using log = Uart<uart::UartPort::usart1>;
	log& getLog() { return *log::instance(); }

public:
    CommandThread();
	virtual ~CommandThread() {}

    void threadFunc() override;
};