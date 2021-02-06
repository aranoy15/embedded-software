#include <watchdog.h>

IWDG_HandleTypeDef Watchdog::m_hiwdg;

void Watchdog::init(uint32_t timeout)
{
#if (DEBUG)
#warning("USE DEBUG")
#endif

#if (!DEBUG)
	m_hiwdg.Instance = IWDG;
	m_hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
	m_hiwdg.Init.Reload = uint32_t(float(timeout) / 32 * 4095);
	HAL_IWDG_Init(&m_hiwdg);
#endif
}

void Watchdog::start()
{
#if (!DEBUG)
	__HAL_IWDG_START(&m_hiwdg);
#endif
}

void Watchdog::reload()
{
#if (!DEBUG)
	HAL_IWDG_Refresh(&m_hiwdg);
#endif
}

void Watchdog::rebootDevice()
{
#if (!DEBUG)
	init(0);
	reload();

	while (true) {
	}
#endif
}