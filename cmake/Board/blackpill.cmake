set(CHIP STM32F401xC)
set(FAMILY F4)
set(ARCHITECTURE ARM_CM4F)

add_definitions(-D${CHIP})
add_definitions(-D${FAMILY})

set(PLATFORM_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mabi=aapcs")