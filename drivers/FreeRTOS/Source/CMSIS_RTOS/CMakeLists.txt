target_sources(FreeRTOS PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/cmsis_os.c
)

target_include_directories(FreeRTOS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)