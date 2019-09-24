target_sources(lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/usb.cpp
)

target_include_directories(lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)