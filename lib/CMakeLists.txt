set(LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/watchdog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uartbuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/os.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mutex.cpp
)

add_library(lib ${LIB_SOURCES})

if (USE_FREERTOS)
    target_sources(lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thread.cpp
    )
endif()

if (USE_MHZ19)
    target_sources(lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mhz19.cpp
    )
endif()

if (USE_BME280)
    target_sources(lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/bme280.cpp
    )
endif()

if (USE_LCD)
    target_sources(lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lcd.cpp
    )
endif()

if (USE_FLASH)
    target_sources(lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/flash.cpp
    )
endif()

if (USE_I2CONEWIRE)
    target_sources(lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/i2conewirebase.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/i2cds18b20.cpp
    )
endif()

if (USE_ADS7844)
    target_sources(lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ads7844.cpp
    )
endif()

if (USE_FATFS)
    target_sources(lib PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sdio.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/fatfsclass.cpp
    )
endif()

if (USE_I2CRTC)
    add_subdirectory(rtc)
endif()

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib PRIVATE drivers) #FreeRTOS FatFS