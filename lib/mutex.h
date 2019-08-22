#ifndef MUTEX_H
#define MUTEX_H

#include <lock.h>

template<class MutexImpl>

class MutexBase : public Lockable
{
private:
	MutexImpl m_impl;
	SemaphoreHandle_t m_mid;

	void create()
	{
		m_mid = m_impl.create();
		m_impl.unlock(m_mid);
	}

	MutexBase(const MutexBase&);
	MutexBase operator=(const MutexBase&);

public:
	MutexBase() : m_impl(), m_mid(NULL)
	{
		if (osKernelRunning() == 1) create();
	}

	virtual ~MutexBase()
	{
		if (m_mid != NULL) m_impl.remove(m_mid);
	}

	void lock()
	{
		if (not(osKernelRunning() == 1)) return;
        if(m_mid == NULL) create();
	}

    void unlock()
    {
        if(m_mid == NULL) return;
        m_impl.Unlock(m_mid);
    }
};

struct MutexOneImpl
{
	osMutexId create() const;	
	bool lock(osMutexId mid) const;
	void unlock(osMutexId mid) const;
	void remove(osMutexId mid) const;
};

typedef MutexBase<MutexOneImpl> Mutex;

#endif /* MUTEX_H */
