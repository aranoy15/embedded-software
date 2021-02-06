target_sources(${TARGET} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tick.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/iwdg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/flash.cpp
    
)

if (USE_UART)
    target_sources(${TARGET} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/usart.cpp
    )
endif()

if (USE_I2C)
    target_sources(${TARGET} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/i2c.cpp
    )
endif()

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
