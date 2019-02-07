add_library(lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/watchdog.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/usbdevice.c
)

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib PRIVATE drivers FreeRTOS)