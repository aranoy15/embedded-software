set(USE_FREERTOS 0)
set(USE_UART 0)
set(USE_LCD 1)
set(USE_FLASH 1)
set(USE_I2CONEWIRE 1)
set(USE_ADS7844 1) 
set(USE_FATFS 1)
set(USE_USB 1)
set(USE_RTC 0)
#set(RTC_TYPE builtin)
set(FRION_TYPE 1) # 0 = 407c, 1 = 134a

set(BOOT_TYPE flash)
set(APP_START_ADDR 0x8004000)

add_definitions(-DUSE_FREERTOS=${USE_FREERTOS})
add_definitions(-DAPP_START_ADDR=${APP_START_ADDR})
add_definitions(-DFRION_TYPE=${FRION_TYPE})