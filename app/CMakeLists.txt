
include_directories(
    ${CMAKE_SOURCE_DIR}/${LOGIC}
)
set(TARGET_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.cpp
)

add_executable(${TARGET} ${TARGET_SOURCES})

#add_subdirectory(${CMAKE_SOURCE_DIR}/logic/${LOGIC} build)

target_link_libraries(${TARGET} c_nano)

add_subdirectory(drivers)
add_subdirectory(lib)
add_subdirectory(logic/${LOGIC})

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
