file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty.c "")
add_library(drivers ${CMAKE_CURRENT_BINARY_DIR}/empty.c)

target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

add_subdirectory(STM32${FAMILY}xx_HAL_Driver)

if (HAVE_FREERTOS)
    add_subdirectory(FreeRTOS)
endif()
