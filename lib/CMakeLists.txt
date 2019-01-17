add_library(lib 
    gpio.cpp
    rcc.cpp
    watchdog.cpp
    usbdevice.cpp
)

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib PRIVATE drivers)