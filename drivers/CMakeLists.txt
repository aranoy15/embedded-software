file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty.c "")
add_library(drivers ${CMAKE_CURRENT_BINARY_DIR}/empty.c)

target_include_directories(drivers PUBLIC
    ${CMAKE_SOURCE_DIR}/drivers/CMSIS/Include
)

add_subdirectory(${FAMILY}xx_HAL_Driver)