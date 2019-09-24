
extern "C" {
#include <stdint.h>
#include <stddef.h>
#include <projdefs.h>
#include <FreeRTOSConfig.h>
#include <portable.h>
}

namespace bsp
{
static const size_t mainRamHeapSize = 12 * 1024;
static uint8_t mainRamHeapBlock[mainRamHeapSize];
HeapRegion_t xHeapRegions[] = {{mainRamHeapBlock, mainRamHeapSize},
                               //{(uint8_t *)0x20000000UL, 0x18000},
                               {NULL, 0}};


void initMem()
{
	vPortDefineHeapRegions(xHeapRegions);
}
}