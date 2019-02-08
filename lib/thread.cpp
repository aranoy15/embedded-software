#include <thread.h>

void Thread::ThreadFuncStatic(const void* argument)
{
	Thread *thisThread = (Thread *)argument;
	thisThread->ThreadFunc();

	osThreadTerminate(thisThread->m_id);
}

Thread::Thread(osPriority prio, uint32_t stackSize, const char* name)
    : m_tinfo({const_cast<char*>(name), ThreadFuncStatic, prio, 0, stackSize}),
      m_id(NULL)
{
	if (m_tinfo.stacksize % sizeof(size_t) != 0)
		m_tinfo.stacksize = (stackSize / sizeof(size_t) + 1) * sizeof(size_t);
}

bool Thread::Start()
{
	m_tinfo.stacksize = m_tinfo.stacksize / sizeof(size_t);
	m_id = osThreadCreate(&m_tinfo, this);

	return (m_id != NULL);
}