#pragma once

#include <stdint.h>
#include <stm32f1xx_hal.h>

class Watchdog
{
    private:
        static IWDG_HandleTypeDef m_hiwdg;
    public:
        static void Init(uint32_t timeout = 15);
        static void Start();
        static void Reload();
        static void RebootDevice();
};