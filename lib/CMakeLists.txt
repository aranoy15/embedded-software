set(LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/watchdog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uartbuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lcd.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/mutex.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bme280.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mhz19.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/taskscheduler.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/sdcard.cpp
)

add_library(lib ${LIB_SOURCES})

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(rtc)

target_link_libraries(lib PRIVATE drivers) #FreeRTOS FatFS