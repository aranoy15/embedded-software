set(TARGET_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.cpp
)

add_executable(${TARGET} ${TARGET_SOURCES})

target_link_libraries(${TARGET} drivers)
