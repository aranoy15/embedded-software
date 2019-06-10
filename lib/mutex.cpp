#include <mutex.h>

osMutexId MutexOneImpl::create() const
{
    // osMutexDef_t need only to static allocation
    osMutexId id = osMutexCreate(NULL);
    return id;
}

bool MutexOneImpl::lock(osMutexId mid) const
{
    return (osMutexWait(mid, osWaitForever) == osOK);
}

void MutexOneImpl::unlock(osMutexId mid) const
{
    osMutexRelease(mid);
}

void MutexOneImpl::remove(osMutexId mid) const
{
    osMutexDelete(mid);
}