#include <i2conewirebase.h>
#include <time.h>


I2COnewireBase::I2COnewireBase()
    : m_mutex(),
      address(0),
      searchAddress(),
      searchLastDisrepancy(0),
      searchExhausted(0)
{
	address = baseAddress | bsp::i2conewire::i2cOnewireAddress;
}

I2COnewireBase::~I2COnewireBase() {}

bool I2COnewireBase::channelSelect(uint8_t channel)
{
    uint8_t master[] = { 0xC3, 0xF0 };
	uint8_t result[1];
	uint8_t ch_read = 0xb8;

	switch (channel)
	{
		case 0:
		default:  
			master[1] = 0xf0; 
			ch_read = 0xb8; 
			break;
		case 1: 
			master[1] = 0xe1; 
			ch_read = 0xb1; 
			break;
		case 2: 
			master[1] = 0xd2; 
			ch_read = 0xaa; 
			break;
		case 3: 
			master[1] = 0xc3; 
			ch_read = 0xa3; 
			break;
		case 4: 
			master[1] = 0xb4; 
			ch_read = 0x9c; 
			break;
		case 5: 
			master[1] = 0xa5; 
			ch_read = 0x95; 
			break;
		case 6: 
			master[1] = 0x96; 
			ch_read = 0x8e; 
			break;
		case 7: 
			master[1] = 0x87; 
			ch_read = 0x87; 
			break;
	};
	
    i2c().readData(address, master, sizeof(master), result, sizeof(result));
	
	return result[0] == ch_read;
}

bool I2COnewireBase::detect()
{
    if (not reset()) return false;

    if (not config(DS2482_CONFIG_APU)) return false;

    return true;
}

bool I2COnewireBase::wireReset()
{
    busyWait(true);
    i2c().writeByte(address, 0xB4);
    uint8_t status = busyWait();

    return status & DS2482_STATUS_PPD ? true : false;
}

void I2COnewireBase::wireWriteByte(uint8_t b)
{
    busyWait(true);
    uint8_t master[] = {0xa5, b};

    i2c().writeData(address, master, sizeof(master));
}

uint8_t I2COnewireBase::wireReadByte()
{
    busyWait(true);
    i2c().writeByte(address, 0x96);
    busyWait();
    setReadPtr(PTR_READ);
    return i2c().readByte(address);
}

void I2COnewireBase::wireWriteBit(uint8_t bit)
{
    busyWait(true);

	uint8_t master[] = { 0x87, uint8_t(bit ? 0x80 : 0) };

    i2c().writeData(address, master, sizeof(master));
}

uint8_t I2COnewireBase::wireReadBit()
{
    wireWriteBit(1);
    uint8_t status = busyWait(true);
    return status & DS2482_STATUS_SBR ? 1 : 0;
}

void I2COnewireBase::wireSkip()
{
    wireWriteByte(0xcc);
}

void I2COnewireBase::wireSelect(uint8_t rom[8])
{
    wireWriteByte(0x55);
    for (int i = 0; i < 8; i++)
        wireWriteByte(rom[i]);
}

void I2COnewireBase::wireResetSearch()
{
    searchExhausted = 0;
	searchLastDisrepancy = 0;
	
	for(uint8_t i = 0; i < 8; i++) 
		searchAddress[i] = 0;
}

uint8_t I2COnewireBase::wireSearch(uint8_t newAddr[])
{
    uint8_t i;
	uint8_t direction;
	uint8_t last_zero=0;
	
	if (searchExhausted) 
		return 0;
	
	if (!wireReset()) 
		return 0;

	busyWait(true);
	wireWriteByte(0xf0);
	
	for(i=1;i<65;i++) 
	{
		int romByte = (i-1)>>3;
		int romBit = 1<<((i-1)&7);
		
		if (i < searchLastDisrepancy)
			direction = searchAddress[romByte] & romBit;
		else
			direction = i == searchLastDisrepancy;
		
		busyWait();
		
		uint8_t master[] = { 0x78, uint8_t(direction ? 0x80 : 0) };

        i2c().writeData(address, master, sizeof(master));

		uint8_t status = busyWait();
		
		uint8_t id = status & DS2482_STATUS_SBR;
		uint8_t comp_id = status & DS2482_STATUS_TSB;
		direction = status & DS2482_STATUS_DIR;
		
		if (id && comp_id)
			return 0;
		else
		{ 
			if (!id && !comp_id && !direction)
				last_zero = i;
		}
		
		if (direction)
			searchAddress[romByte] |= romBit;
		else
			searchAddress[romByte] &= (uint8_t)~romBit;
	}

	searchLastDisrepancy = last_zero;

	if (last_zero == 0) 
		searchExhausted = 1;
	
	for (i=0;i<8;i++) 
		newAddr[i] = searchAddress[i];
	
	return 1;      
}

bool I2COnewireBase::config(uint8_t config)
{
	uint8_t master[] = {0xD2, uint8_t(config | (~config << 4))};
	uint8_t result[1];

    i2c().readData(address, master, sizeof(master), result, sizeof(result));

	return (result[0] == config);
}

uint8_t I2COnewireBase::wireReadStatus(bool setPtr)
{
    if (setPtr)
		setReadPtr(PTR_STATUS);
	
	return i2c().readByte(address);
}

void I2COnewireBase::setReadPtr(uint8_t readPtr)
{
    uint8_t master[] = { 0xE1, readPtr };

    i2c().writeData(address, master, sizeof(master));
}

uint8_t I2COnewireBase::busyWait(bool setReadPtr)
{
    uint8_t status;
	int loopCount = 1000;
	while((status = wireReadStatus(setReadPtr)) & DS2482_STATUS_BUSY)
	{
		if (--loopCount <= 0) break;
			
		Time::sleep(Time(20));
	}
	return status;
}

bool I2COnewireBase::reset()
{
    uint8_t master[] = { 0xF0 };
	uint8_t result[1];

    i2c().readData(address, master, sizeof(master), result, sizeof(result));

	return ((result[0] & 0xF7) == 0x10);
}

uint8_t I2COnewireBase::crc8(uint8_t *addr, uint8_t size)
{
	uint8_t crc = 0;

	for (uint8_t i = 0; i < size; i++) {
		uint8_t inbyte = addr[i];
		for (uint8_t j = 0; j < 8; j++) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;

			inbyte >>= 1;
		}
	}
	return crc;
}