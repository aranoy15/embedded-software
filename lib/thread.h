#pragma once

#include <bsp.h>
#include <functional>

class Thread
{
private:
	uint8_t mPrio;
	uint16_t mStackSize;
	TaskHandle_t mHandle;

	static void threadFuncStatic(void* argument);

public:
	Thread(const Thread&) = delete;
	Thread& operator=(const Thread&) = delete;

	Thread(uint8_t prio, uint32_t stackSize);
	virtual ~Thread(){}

	virtual const char* threadName() { return "noname"; }
	uint8_t prio() const { return mPrio; }

	bool start();
	static bool start(void (*func)(void*),
	                  const char* threadName, uint32_t stackSize,
	                  uint8_t priority);

protected:
	virtual void threadFunc() = 0;
};
