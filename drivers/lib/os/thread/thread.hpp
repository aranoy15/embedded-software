#ifndef DRIVERS_LIB_OS_THREAD_THREAD_BASE
#define DRIVERS_LIB_OS_THREAD_THREAD_BASE

#include <bsp.hpp>
#include <string>

namespace lib::os::thread
{
class Thread
{
	static void func_static(void* argument);

public:
	Thread(const Thread&) = delete;
	Thread& operator=(const Thread&) = delete;

	Thread(bsp::os::thread::priority_t prio, std::size_t stack);
	virtual ~Thread() {}

	virtual std::string_view name() const { return "noname"; }

	bool start();
	static bool start(bsp::os::thread::func_t f, std::string_view name,
	                  std::size_t stack, bsp::os::thread::priority_t prio);

protected:
	virtual void func() = 0;

private:
	bsp::os::thread::priority_t _prio;
	std::size_t _stack;
	bsp::os::thread::id_t _id;
};
}  // namespace lib::os::thread

#endif /* DRIVERS_LIB_OS_THREAD_THREAD_BASE */
