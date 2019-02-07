#include <watchdog.h>

IWDG_HandleTypeDef Watchdog::m_hiwdg;

void Watchdog::Init(uint32_t timeout)
{
#if !DBG
	m_hiwdg.Instance = IWDG;
	m_hiwdg.Init.Prescaler = IWDG_PRESCALER_256;
	m_hiwdg.Init.Reload = uint32_t(float(timeout) / 32 * 4095);
	HAL_IWDG_Init(&m_hiwdg);
#endif
}

void Watchdog::Start()
{
#if !DBG
	__HAL_IWDG_START(&m_hiwdg);
#endif
}

void Watchdog::Reload()
{
#if !DBG
	HAL_IWDG_Refresh(&m_hiwdg);
#endif
}

void Watchdog::RebootDevice()
{
#if !DBG
	Init(0);
	Reload();

	while (true) {
	}
#endif
}