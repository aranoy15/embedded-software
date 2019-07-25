#include <watchdog.h>

IWDG_HandleTypeDef Watchdog::m_hiwdg;

void Watchdog::init(uint32_t timeout)
{
	m_hiwdg.Instance = IWDG;
	m_hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
	m_hiwdg.Init.Reload = uint32_t(float(timeout) / 32 * 4095);
	HAL_IWDG_Init(&m_hiwdg);
}

void Watchdog::start()
{
	__HAL_IWDG_START(&m_hiwdg);
}

void Watchdog::reload()
{
	HAL_IWDG_Refresh(&m_hiwdg);
}

void Watchdog::rebootDevice()
{
	init(0);
	reload();

	while (true) {
	}
}