add_subdirectory(${BOARD})
add_subdirectory(bsp)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(lib)
