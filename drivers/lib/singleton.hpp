#ifndef DRIVERS_LIB_SINGLETON
#define DRIVERS_LIB_SINGLETON

#include <cstdlib>
#include <typeinfo>
#include <memory>
#include <bsp.hpp>

#if (USE_OS)
#include <lib/os/mutex.hpp>
#endif

template <class T>
class Singleton
{
public:
	Singleton(const Singleton&) = delete;
	const Singleton& operator=(Singleton&) = delete;

	virtual ~Singleton() {}

	static bool is_instance() { return _instance != nullptr; }

	static std::unique_ptr<T>& instance()
	{
		if (not _instance) {
#if (USE_OS)
			if (bsp::os::is_running()) {
				lib::Lock lock(_mutex);
				if (not is_instance()) create();
			} else
				create();
#else
			create();
#endif
		}

		return _instance;
	}

	static void delete_instance()
	{
#if (USE_OS)
		lib::Lock lock(_mutex);
#endif
		if (is_instance()) _instance.reset();
	}

protected:
    Singleton() {}

private:
	inline static std::unique_ptr<T> _instance = nullptr;

#if (USE_OS)
	inline static lib::os::Mutex _mutex;
#endif

private:
	static void create() { _instance = std::make_unique<T>(); }
};

#endif /* DRIVERS_LIB_SINGLETON */
