target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/interrupts.cpp
)

add_subdirectory(threads)