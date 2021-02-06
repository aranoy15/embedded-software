target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/mutex.cpp
)

add_subdirectory(thread)
