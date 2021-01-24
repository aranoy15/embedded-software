#include <os.hpp>
#include <main.h>

#include <FreeRTOS.h>
#include <task.h>

namespace bsp::os
{

void init()
{
	osKernelInitialize();
}

void start()
{
	osKernelStart();
}

bool is_running()
{
	return osKernelGetState() == osKernelRunning;
}

}  // namespace bsp::os

namespace bsp::os::thread
{

auto create(func_t func, void* args, std::string_view name, std::size_t stack,
            Priority prio) -> id_t
{
	using attr_t = osThreadAttr_t;

	attr_t attributes = {0};

	attributes.name = name.data();
	attributes.stack_size = stack * sizeof(std::size_t);
	attributes.priority = static_cast<osPriority_t>(prio);

	return osThreadNew(func, args, &attributes);
}

void remove(id_t)
{
	osThreadExit();
}

}  // namespace bsp::os::thread

namespace bsp::os::mutex
{
auto create(std::string_view name) -> mutex_id_t
{
	using attr_t = osMutexAttr_t;

	attr_t attributes = {0};

	return osMutexNew(&attributes);
}

bool take(mutex_id_t handle, std::uint32_t timeout)
{
	return osMutexAcquire(handle, timeout) == osOK;
}

void give(mutex_id_t handle)
{
	osMutexRelease(handle);
}

void remove(mutex_id_t handle)
{
	osMutexDelete(handle);
}
}  // namespace bsp::os::mutex

extern "C" void* malloc(size_t size)
{
	return pvPortMalloc(size);
}


extern "C" void free(void* block)
{
	return vPortFree(block);
}

void* operator new(std::size_t size)
{
	void* block = malloc(size);
	return block;
}


void operator delete(void* block) 
{
	free(block);
}

void operator delete(void* block, std::size_t) 
{
	free(block);
}

void *operator new[](std::size_t size)
{
	void* block = malloc(size);
	return block;
}

void operator delete[](void* block)
{
	free(block);
}

void operator delete[](void* block, std::size_t)
{
	free(block);
}
