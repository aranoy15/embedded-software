#pragma once

#include <ff.h>
#include <diskio.h>

#include <singleton.h>
#include <spi.h>

class FatFsClass : public Singleton<FatFsClass>
{
private:
    FATFS ffs;
    uint8_t ffsResult;

public:
	Spi<spi::spiPort1>& getSpi() const
	{
		return *Spi<spi::spiPort1>::instance();
	}

public:
	FatFsClass() : ffsResult(0) {}

	void init();

};