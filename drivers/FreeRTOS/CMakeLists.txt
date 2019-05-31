add_library(FreeRTOS
    croutine.c
    event_groups.c
    list.c
    queue.c
    tasks.c
    timers.c
)

target_include_directories(FreeRTOS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/logic/config
    ${CMAKE_SOURCE_DIR}/drivers/CMSIS/include
)

add_subdirectory(CMSIS_RTOS)
add_subdirectory(portable)