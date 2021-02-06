if (USE_I2C_DATETIME)
    add_subdirectory(datetime)
endif()

add_subdirectory(sensors)

if (USE_I2C_LCD)
    add_subdirectory(lcd)
endif()
