#pragma once

#include <stm32f1xx_hal.h>

namespace stm32system
{
#define O_ASSERT(x)                                                            \
	while (not x) {                                                            \
	}
void init();
}

/*
#pragma once
#include <stm32f1xx_hal.h>

#define O_ASSERT(x)                                                            \
	while (! x) {                                                            \
	}
void RccInit();
*/