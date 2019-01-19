add_library(lib 
    rcc.c
    gpio.c
)

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(lib PRIVATE drivers)