file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/empty.c "")
add_library(drivers ${CMAKE_CURRENT_BINARY_DIR}/empty.c)

target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/STM32${FAMILY}
)

add_subdirectory(STM32${FAMILY}xx_HAL_Driver)

add_subdirectory(CSP)
add_subdirectory(BSP)

if (USE_FREERTOS)
    add_subdirectory(FreeRTOS)
endif ()
#add_subdirectory(FatFS)