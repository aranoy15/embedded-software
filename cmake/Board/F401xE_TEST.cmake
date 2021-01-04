set(CHIP STM32F401xE)
set(FAMILY F4)
set(ARCHITECTURE ARM_CM4)
set(APP_START_ADDR 0x8000000)
set(HAVE_BOOT FALSE)

add_definitions(-D${CHIP})
add_definitions(-D${FAMILY})
add_definitions(-DAPP_START_ADDR=${APP_START_ADDR})

set(PLATFORM_FLAGS "-mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mabi=aapcs ")
