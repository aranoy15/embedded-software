#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex.h>

template <class T>
class Singleton
{
private:
	static T* mInstance;
	static Mutex mMutex;

	Singleton(const Singleton&);
	const Singleton& operator=(Singleton&);

	static void create() { mInstance = new T; }

protected:
	Singleton() {}

public:
	virtual ~Singleton() {}

	static bool isInstance() { return mInstance != nullptr; }

	static T* instance()
	{
		if (not mInstance) {
			if (osRunning()) {
				Lock lock(mMutex);
				if (not mInstance) create();
			} else
				create();
		}
		return mInstance;
	}

	static void deleteInstance()
	{
		if (mInstance) delete mInstance;
		mInstance = nullptr;
	}
};

template <class T>
T* Singleton<T>::mInstance = nullptr;

template <class T>
Mutex Singleton<T>::mMutex;

#endif /* SINGLETON_H */