#include <drivers/bsp/F401xE_TEST/os.hpp>
#include <drivers/F401xE_TEST/Core/Inc/main.h>

#include <drivers/F401xE_TEST/Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h>
#include <drivers/F401xE_TEST/Middlewares/Third_Party/FreeRTOS/Source/include/task.h>

void bsp::os::init()
{
    osKernelInitialize();
}

void bsp::os::start()
{
    osKernelStart();
}

auto bsp::os::reg(func_t func, void* args, std::string_view name, std::size_t stack, Priority prio) -> handle_t
{
    using attr_t = osThreadAttr_t;

    attr_t attributes = {0};

    attributes.name = name.data();
    attributes.stack_size = stack * sizeof(std::size_t);
    attributes.priority = static_cast<osPriority_t>(prio);

    return osThreadNew(func, args, &attributes);
}

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
