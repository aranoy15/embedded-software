#include <mutex.h>

#if (USE_FREERTOS)
SemaphoreHandle_t MutexOneImpl::create() const
{
	return xSemaphoreCreateMutex();
}


bool MutexOneImpl::lock(SemaphoreHandle_t sem) const
{
	return xSemaphoreTake(sem, portMAX_DELAY) == pdTRUE;
}


void MutexOneImpl::unlock(SemaphoreHandle_t sem) const
{
	xSemaphoreGive(sem);
}


void MutexOneImpl::remove(SemaphoreHandle_t sem) const
{
	vSemaphoreDelete(sem);
}


SemaphoreHandle_t MutexReImpl::create() const
{
	return xSemaphoreCreateRecursiveMutex();
}


bool MutexReImpl::lock(SemaphoreHandle_t sem) const
{
	return xSemaphoreTakeRecursive(sem, portMAX_DELAY) == pdTRUE;
}


void MutexReImpl::unlock(SemaphoreHandle_t sem) const
{
	xSemaphoreGiveRecursive(sem);
}


void MutexReImpl::remove(SemaphoreHandle_t sem) const
{
	vSemaphoreDelete(sem);
}
#endif