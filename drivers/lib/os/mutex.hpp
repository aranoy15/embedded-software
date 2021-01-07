#ifndef DRIVERS_LIB_OS_MUTEX
#define DRIVERS_LIB_OS_MUTEX

#include <drivers/lib/lock.hpp>
#include <bsp.hpp>

namespace lib::os
{

template <class MutexImpl>
class MutexBase : public Lockable
{
public:
	using mutex_impl_t = MutexImpl;

private:
	void create()
	{
		_id = _impl.create();
		_impl.unlock(_id);
	}

public:
	MutexBase(const MutexBase&) = delete;
	MutexBase operator=(const MutexBase&) = delete;

	MutexBase() : _impl(), _id()
	{
		// if (bsp::os::is_running()) create();
	}

	virtual ~MutexBase()
	{
		if (not _id) _impl.remove(_id);
	}

	void lock()
	{
		if (not bsp::os::is_running()) return;
		if (not _id) create();

		_impl.lock(_id);
	}

	void unlock()
	{
		if (not _id) return;
		_impl.unlock(_id);
	}

private:
	bsp::os::mutex::mutex_id_t _id;
	mutex_impl_t _impl;
};

struct MutexImpl
{
	bsp::os::mutex::mutex_id_t create() const;
	bool lock(bsp::os::mutex::mutex_id_t id) const;
	void unlock(bsp::os::mutex::mutex_id_t id) const;
	void remove(bsp::os::mutex::mutex_id_t id) const;
};

using Mutex = MutexBase<MutexImpl>;
}  // namespace lib::os

#endif /* DRIVERS_LIB_OS_MUTEX */
