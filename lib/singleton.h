#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex.h>

template <class T>
class Singleton
{
private:
	static T* m_instance;
	static Mutex m_mutex;

	Singleton(const Singleton&);
	const Singleton& operator=(Singleton&);

	static void Create() { m_instance = new T; }

protected:
	Singleton() {}

public:
	virtual ~Singleton() {}

	static bool IsInstance() { return m_instance != NULL; }

	static T* Instance()
	{
		if (not m_instance) {
			if (osKernelRunning()) {
				Lock lock(m_mutex);
				if (not m_instance) Create();
			} else {
				Create();
			}
		}
		return m_instance;
	}

	static void DeleteInstance()
	{
		Lock lock(m_mutex);
		if (m_instance) delete m_instance;
		m_instance = NULL;
	}
};

template <class T>
T* Singleton<T>::m_instance = NULL;

template <class T>
Mutex Singleton<T>::m_mutex;

#endif /* SINGLETON_H */