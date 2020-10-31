target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/usart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/info.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tick.cpp
    ${CMAKE_SOURCE_DIR}/chip/${CHIP}/startup_stm32f103xb.s
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
