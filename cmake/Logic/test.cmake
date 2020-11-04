
set(USE_STREAM 0)
set(USE_UART 0)
set(USE_MHZ19 0)
set(USE_DATETIME 0)
set(USE_I2C 0)
set(USE_I2C_DATETIME 0)
set(USE_BME280 0)
set(USE_TASK 0)
set(USE_I2C_LCD 0)

if (USE_UART)
    set(USE_LOG 0)
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
