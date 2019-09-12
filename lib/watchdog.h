#pragma once

#include <stdint.h>
#include <bsp.h>

class Watchdog
{
    private:
        static IWDG_HandleTypeDef m_hiwdg;
    public:
        static void init(uint32_t timeout = 15);
        static void start();
        static void reload();
        static void rebootDevice();
};