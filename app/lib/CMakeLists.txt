target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if (USE_STREAM)
    add_subdirectory(stream)
endif()

if (USE_UART)
    add_subdirectory(uart)
endif()

add_subdirectory(time)

if (USE_DATETIME)
    add_subdirectory(datetime)
endif()

if (USE_I2C)
    add_subdirectory(i2c)
endif()

if (USE_TASK)
    add_subdirectory(task)
endif()

if (USE_OS)
    add_subdirectory(os)
endif()
