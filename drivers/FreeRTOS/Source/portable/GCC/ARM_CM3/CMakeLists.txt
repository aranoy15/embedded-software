target_sources(FreeRTOS PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/port.c
)

target_include_directories(FreeRTOS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)