#include <mutex.h>

osMutexId MutexOneImpl::Create() const
{
    // osMutexDef_t need only to static allocation
    osMutexId id = osMutexCreate(NULL);
    return id;
}

bool MutexOneImpl::Lock(osMutexId mid) const
{
    return (osMutexWait(mid, osWaitForever) == osOK);
}

void MutexOneImpl::Unlock(osMutexId mid) const
{
    osMutexRelease(mid);
}

void MutexOneImpl::Remove(osMutexId mid) const
{
    osMutexDelete(mid);
}