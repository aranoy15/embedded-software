set(CHIP STM32F103xB)
set(FAMILY F1)
set(ARCHITECTURE ARM_CM3)
set(BOOT_DRIVER F103xB)
set(BOOT_START_ADDR 0x8000000)

add_definitions(-D${CHIP})
add_definitions(-D${FAMILY})

set(PLATFORM_FLAGS "-mthumb -mcpu=cortex-m3 -mabi=aapcs ")