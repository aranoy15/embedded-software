add_library(lib 
    ${CMAKE_CURRENT_SOURCE_DIR}/rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usbdevice.c
)

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib PRIVATE drivers)