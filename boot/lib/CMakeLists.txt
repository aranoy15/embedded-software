target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/uart.cpp
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
