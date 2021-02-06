
extern "C" {
#include <stdint.h>
#include <stddef.h>
#include <projdefs.h>
#include <FreeRTOSConfig.h>
#include <portable.h>
}

namespace bsp
{
static const size_t mainRamHeapSize = 56 * 1024;
static uint8_t mainRamHeapBlock[mainRamHeapSize];
HeapRegion_t xHeapRegions[] = {{mainRamHeapBlock, mainRamHeapSize},
                               {NULL, 0}};


void initMem()
{
	vPortDefineHeapRegions(xHeapRegions);
}
}