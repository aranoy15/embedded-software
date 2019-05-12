#ifndef SINGLETON_H
#define SINGLETON_H

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
			Create();
		}
		return m_instance;
	}

	static void deleteInstance()
	{
		if (m_instance) delete m_instance;
		m_instance = nullptr;
	}
};

template <class T>
T* Singleton<T>::m_instance = nullptr;

#endif /* SINGLETON_H */