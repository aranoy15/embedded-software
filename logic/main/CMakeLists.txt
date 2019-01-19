
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set(TARGET_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.c
)

add_executable(${TARGET} ${TARGET_SOURCES})

target_link_libraries(${TARGET} drivers lib)
