
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set(TARGET_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupts.cpp
)

add_executable(${TARGET} ${TARGET_SOURCES})

target_link_libraries(${TARGET} drivers lib c_nano)

#if (HAVE_FREERTOS)
#    target_link_libraries(${TARGET} FreeRTOS)
#endif()
