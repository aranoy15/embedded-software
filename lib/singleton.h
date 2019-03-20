#ifndef SINGLETON_H
#define SINGLETON_H

#if HAVE_FREERTOS
#include <mutex.h>
#endif

template <class T>
class Singleton
{
private:
	static T* m_instance;

	#if HAVE_FREERTOS
	static Mutex m_mutex;
	#endif

	Singleton(const Singleton&);
	const Singleton& operator=(Singleton&);

	static void Create() { m_instance = new T; }

protected:
	Singleton() {}

public:
	virtual ~Singleton() {}

	static bool isInstance() { return m_instance != nullptr; }

	static T* instance()
	{
		if (not m_instance) {
			#if HAVE_FREERTOS
			if (osKernelRunning()) {
				Lock lock(m_mutex);
				if (not m_instance) Create();
			} else {
			#endif
				Create();
			#if HAVE_FREERTOS
			}
			#endif
		}
		return m_instance;
	}

	static void deleteInstance()
	{
		#if HAVE_FREERTOS
		Lock lock(m_mutex);
		#endif

		if (m_instance) delete m_instance;
		m_instance = nullptr;
	}
};

template <class T>
T* Singleton<T>::m_instance = nullptr;

#if HAVE_FREERTOS
template <class T>
Mutex Singleton<T>::m_mutex;
#endif

#endif /* SINGLETON_H */