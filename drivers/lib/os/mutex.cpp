#include <lib/os/mutex.hpp>

namespace lib::os
{
bsp::os::mutex::mutex_id_t MutexImpl::create() const
{
	return bsp::os::mutex::create();
}

bool MutexImpl::lock(bsp::os::mutex::mutex_id_t id) const
{
	return bsp::os::mutex::take(id);
}

void MutexImpl::unlock(bsp::os::mutex::mutex_id_t id) const
{
	bsp::os::mutex::give(id);
}

void MutexImpl::remove(bsp::os::mutex::mutex_id_t id) const
{
	bsp::os::mutex::remove(id);
}
}  // namespace lib::os
