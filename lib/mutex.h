#ifndef MUTEX_H
#define MUTEX_H

#include <cmsis_os.h>
#include <lock.h>

template<class MutexImpl>

class MutexBase : public Lockable
{
private:
	MutexImpl m_impl;
	SemaphoreHandle_t m_mid;

	void Create()
	{
		m_mid = m_impl.Create();
		m_impl.Unlock(m_mid);
	}

	MutexBase(const MutexBase&);
	MutexBase operator=(const MutexBase&);

public:
	MutexBase() : m_impl(), m_mid(NULL)
	{
		if (osKernelRunning() == 1) Create();
	}

	virtual ~MutexBase()
	{
		if (m_mid != NULL) m_impl.Remove(m_mid);
	}

	void Lock()
	{
		if (not(osKernelRunning() == 1)) return;
        if(m_mid == NULL) Create();
	}

    void Unlock()
    {
        if(m_mid == NULL) return;
        m_impl.Unlock(m_mid);
    }
};

struct MutexOneImpl
{
	osMutexId Create() const;	
	bool Lock(osMutexId mid) const;
	void Unlock(osMutexId mid) const;
	void Remove(osMutexId mid) const;
};

typedef MutexBase<MutexOneImpl> Mutex;

#endif /* MUTEX_H */
