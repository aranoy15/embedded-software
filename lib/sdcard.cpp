#include <sdcard.h>

extern "C" int disk_status()
{
    return 0;
}

extern "C" int sd_initialize()
{
    return 0;
}

extern "C" int sd_disk_read(uint8_t* buff, uint32_t sector, uint32_t count)
{
	uint8_t* b = buff;

    for (uint32_t n = 0; n < count; n++) {
    }
}

void FatFsClass::init()
{
    getSpi().init();
}