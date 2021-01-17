#include <drivers/lib/os/thread/thread.hpp>

using namespace lib::os::thread;

void Thread::func_static(void* argument)
{
	Thread *this_thread = (Thread*)(argument);
	this_thread->func();

	bsp::os::thread::remove(this_thread->_id);
}

Thread::Thread(bsp::os::thread::priority_t prio, std::size_t stack)
    : _prio(prio), _stack(stack), _id()
{
}

bool Thread::start()
{
	_id = bsp::os::thread::create(func_static, this, name(), _stack, _prio);
	if (not _id) return false;

	return true;
}


bool Thread::start(bsp::os::thread::func_t f, std::string_view name,
                   std::size_t stack, bsp::os::thread::priority_t prio)
{
	bsp::os::thread::id_t id = bsp::os::thread::create(f, nullptr, name, stack, prio);

	return id;
}
