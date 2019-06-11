target_sources(lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/datetime.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/timespan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ds3231.cpp
)

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)