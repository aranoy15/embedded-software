#include <drivers/bsp/F401xE_TEST/os.hpp>
#include <drivers/csp/F401xE_TEST/Core/Inc/main.h>

#include <drivers/csp/F401xE_TEST/Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h>
#include <drivers/csp/F401xE_TEST/Middlewares/Third_Party/FreeRTOS/Source/include/task.h>

void bsp::os::init()
{
    osKernelInitialize();
}

void bsp::os::start()
{
    osKernelStart();
}

bool bsp::os::is_running()
{
	return osKernelGetState() == osKernelRunning;
}

auto bsp::os::thread::create(func_t func, void* args, std::string_view name,
                             std::size_t stack, Priority prio) -> id_t
{
	using attr_t = osThreadAttr_t;

	attr_t attributes = {0};

	attributes.name = name.data();
	attributes.stack_size = stack * sizeof(std::size_t);
	attributes.priority = static_cast<osPriority_t>(prio);

	return osThreadNew(func, args, &attributes);
}

void bsp::os::thread::remove(id_t)
{
	osThreadExit();
}

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
	//if (not block) throw std::bad_alloc();
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
	//if (not block) throw std::bad_alloc();
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
