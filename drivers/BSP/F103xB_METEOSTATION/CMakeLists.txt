target_sources(${TARGET} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/usart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/i2c.cpp
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
