#ifndef DRIVERS_BSP_F103XB_METEOSTATION_FLASH
#define DRIVERS_BSP_F103XB_METEOSTATION_FLASH

#include <cinttypes>

namespace bsp::flash
{
constexpr std::uint32_t start_address = 0x0801FC00; /* Base @ of Page 127, 1 Kbytes */
void erase();
void write(std::uint32_t address, std::uint8_t data[], std::size_t size);
void read(std::uint32_t address, std::uint8_t data[], std::size_t size);
}

#endif /* DRIVERS_BSP_F103XB_METEOSTATION_FLASH */
