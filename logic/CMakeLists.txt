
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set(TARGET_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.cpp
)

add_executable(${TARGET} ${TARGET_SOURCES})

add_subdirectory(${LOGIC})

target_link_libraries(${TARGET} drivers lib c_nano FreeRTOS)
