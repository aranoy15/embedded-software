target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/mainthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/blinkthread.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/commandthread.cpp
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)