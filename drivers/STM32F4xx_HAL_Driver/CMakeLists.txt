target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/system_stm32f4xx.c
)

target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(Src)