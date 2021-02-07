target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/wake.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timer.cpp
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
