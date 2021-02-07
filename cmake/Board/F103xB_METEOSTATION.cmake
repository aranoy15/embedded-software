set(CHIP STM32F103xB)
set(FAMILY F1)
set(ARCHITECTURE ARM_CM3)
set(APP_START_ADDR 0x8008000)
set(HAVE_BOOT TRUE)
set(BOOT_START_ADDR 0x8000000)
set(BOOT_DRIVER F103xB)
set(BOOT_TYPE uart)
#set(BOOT_TYPE usb)

add_definitions(-D${CHIP})
add_definitions(-D${FAMILY})
add_definitions(-DAPP_START_ADDR=${APP_START_ADDR})
add_definitions(-DUSE_HAL_DRIVER)

set(PLATFORM_FLAGS "-mthumb -mcpu=cortex-m3 -mabi=aapcs ")