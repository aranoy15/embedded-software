#ifndef THREAD_H
#define THREAD_H

#include <cmsis_os.h>

class Thread
{
private:
	osThreadDef_t m_tinfo;
	osThreadId m_id;

	static void ThreadFuncStatic(const void* argument);
	

private:
	Thread(const Thread&);
	Thread operator=(const Thread&);

public:
	Thread(osPriority prio, uint32_t stackSize, const char* name);
	    
	virtual ~Thread(){}

	bool Start();

protected:
	virtual void ThreadFunc() {};
};

#endif /* THREAD_H */
