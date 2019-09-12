#pragma once

#include <lock.h>
#include <bsp.h>
#include <os.h>

template<class MutexImpl>

class MutexBase : public Lockable
{
private:
#if (USE_FREERTOS)
	MutexImpl m_impl;
	SemaphoreHandle_t m_mid;
#endif

	void create()
	{
#if (USE_FREERTOS)
		m_mid = m_impl.create();
		m_impl.unlock(m_mid);
#endif
	}

public:

	MutexBase(const MutexBase&) = delete;
	MutexBase operator=(const MutexBase&) = delete;

	MutexBase() 
#if (USE_FREERTOS)
	: m_impl(), 
	  m_mid(NULL)
#endif
	{
#if (USE_FREERTOS)
		if (osRunning() == 1) create();
#endif
	}

	virtual ~MutexBase()
	{
#if (USE_FREERTOS)
		if (m_mid != NULL) m_impl.remove(m_mid);
#endif
	}

	void lock()
	{
#if (USE_FREERTOS)
		if (not osRunning()) return;
		if (m_mid == NULL) create();
#endif
	}

    void unlock()
    {
#if (USE_FREERTOS)
		if (m_mid == NULL) return;
		m_impl.unlock(m_mid);
#endif
	}
};

struct MutexOneImpl
{
#if (USE_FREERTOS)
	SemaphoreHandle_t create() const;
	bool lock(SemaphoreHandle_t sem) const;
	void unlock(SemaphoreHandle_t sem) const;
	void remove(SemaphoreHandle_t sem) const;
#endif
};


struct MutexReImpl
{
#if (USE_FREERTOS)
	SemaphoreHandle_t create() const;
	bool lock(SemaphoreHandle_t sem) const;
	void unlock(SemaphoreHandle_t sem) const;
	void remove(SemaphoreHandle_t sem) const;
#endif
};


typedef MutexBase<MutexOneImpl> Mutex;
typedef MutexBase<MutexReImpl> MutexRe;

