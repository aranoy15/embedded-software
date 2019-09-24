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


bool Thread::start(void (*func)(void*), const char* threadName, uint32_t stackSize, uint8_t priority)
{
	if (stackSize % sizeof(size_t) != 0)
		stackSize = (stackSize / sizeof(size_t) + 1) * sizeof(size_t);	

	BaseType_t status =
	    xTaskCreate(func, threadName, stackSize / sizeof(size_t), NULL, priority, NULL);

	return (status == pdPASS);
}