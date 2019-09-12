#include <thread.h>

void Thread::threadFuncStatic(void* argument)
{
	Thread *thisThread = (Thread *)argument;
	thisThread->threadFunc();

	vTaskDelete(thisThread->mHandle);
}

Thread::Thread(uint8_t prio, uint32_t stackSize)
    : mPrio(prio), mStackSize(stackSize), mHandle()
{
	if (mStackSize % sizeof(size_t) != 0)
		mStackSize = (stackSize / sizeof(size_t) + 1) * sizeof(size_t);
}

bool Thread::start()
{
	BaseType_t status =
	    xTaskCreate(threadFuncStatic, threadName(), mStackSize / sizeof(size_t), this, prio(), &mHandle);
	if (status != pdPASS) return false;

	return true;
}