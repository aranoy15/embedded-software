target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/port.c
)

target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)