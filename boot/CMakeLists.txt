SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections ${PLATFORM_FLAGS} ${SPECS} -T ${CMAKE_CURRENT_SOURCE_DIR}/${CHIP}/link.ld")

set(TARGET boot)

add_executable(${TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/bootuart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bootuart.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/bootflash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sleep.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bootinfo.cpp
)

target_include_directories(boot PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib
)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

target_link_libraries(${TARGET} drivers c_nano)

add_subdirectory(${CHIP})

size(${TARGET})
generate_bin(${TARGET})
flash(${TARGET} 0x8000000)