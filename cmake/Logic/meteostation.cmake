set(USE_STREAM 1)
set(USE_UART 1)
set(USE_LOG 1)
set(USE_MHZ19 1)
set(USE_DATETIME 1)
set(USE_I2C 1)
set(USE_I2C_DATETIME 1)
set(USE_BME280 1)
set(USE_TASK 1)
set(USE_I2C_LCD 1)

set(USE_OS 0)

if (USE_LOG)
    set(USE_UART 1)
    set(USE_STREAM 1)
endif()

if (USE_MHZ19)
    set(USE_UART 1)
endif()

if (USE_I2C_DATETIME)
    set(USE_DATETIME 1)
endif()

if (USE_BME280)
    set(USE_I2C 1)
endif()

if (USE_I2C_LCD)
    set(USE_I2C 1)
endif()