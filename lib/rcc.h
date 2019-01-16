#pragma once
#include <stm32f1xx_hal.h>

namespace rcc
{
#define O_ASSERT(x)                                                            \
	while (not x) {                                                            \
	}
void Init();
}