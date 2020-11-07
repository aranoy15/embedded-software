#include <drivers/bsp/F103xB_METEOSTATION/flash.hpp>
#include <main.h>

void bsp::flash::erase()
{
    FLASH_EraseInitTypeDef erase_inits;
    uint32_t errors = 0;
    std::uint32_t start = start_address;
    std::uint32_t count = 1;

    erase_inits.TypeErase = FLASH_TYPEERASE_PAGES;
	erase_inits.PageAddress = start;
	erase_inits.NbPages = count;

    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&erase_inits, &errors);
    HAL_FLASH_Lock();
}

void bsp::flash::write(std::uint32_t address, std::uint8_t data[], std::size_t size)
{
    HAL_FLASH_Unlock();

    for (uint32_t i = 0; i < size; i += sizeof(uint32_t)) {
		uint32_t current_address = address + i;
		uint32_t word_to_write = *(uint32_t*)(data + i);

		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, current_address, word_to_write);
	}

    HAL_FLASH_Lock();
}

void bsp::flash::read(std::uint32_t address, std::uint8_t data[], std::size_t size)
{
	uint8_t *int_vector_table = (uint8_t *)address;

	for (std::size_t i = 0; i < size; ++i) {
        data[i] = int_vector_table[i];
    }
}
