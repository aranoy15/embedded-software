target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx_hal_gpio_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx_hal_iwdg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx_hal_pwr.c
)