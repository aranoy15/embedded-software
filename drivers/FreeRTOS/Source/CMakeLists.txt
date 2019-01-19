set(FREERTOS_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/timers.c
)

add_library(FreeRTOS ${FREERTOS_SOURCES})

target_include_directories(FreeRTOS PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(CMSIS_RTOS)
add_subdirectory(portable)

target_link_libraries(FreeRTOS PRIVATE drivers)