#ifndef DRIVERS_BSP_F401XE_TEST_OS
#define DRIVERS_BSP_F401XE_TEST_OS

#include <drivers/csp/F401xE_TEST/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.h>
#include <string>

namespace bsp::os
{
void init();
void start();
bool is_running();
}

namespace bsp::os::thread
{
using func_t = osThreadFunc_t;
using id_t = osThreadId_t;

enum class Priority
{
	Idle = osPriorityIdle,
	Low = osPriorityLow,
	Normal = osPriorityNormal,
	High = osPriorityHigh,
	Realtime = osPriorityRealtime
};

using priority_t = Priority;

id_t create(func_t func, void* args, std::string_view name, std::size_t stack,
            Priority prio);
void remove(id_t id);
}  // namespace bsp::os

namespace bsp::os::mutex
{
using mutex_id_t = osMutexId_t;
constexpr std::uint32_t wait_forever = osWaitForever;

mutex_id_t create(std::string_view name = "");
bool take(mutex_id_t handle, std::uint32_t timeout = wait_forever);
void give(mutex_id_t handle);
void remove(mutex_id_t handle);
}  // namespace bsp::os::mutex

#endif /* DRIVERS_BSP_F401XE_TEST_OS */
