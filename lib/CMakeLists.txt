set(LIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/system.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/watchdog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uartbuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lcd.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/mutex.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/thread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/spi.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/uart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
)

add_library(lib ${LIB_SOURCES})

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib PRIVATE drivers) #FreeRTOS