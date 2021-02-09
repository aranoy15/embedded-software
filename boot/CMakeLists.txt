SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections ${PLATFORM_FLAGS} ${SPECS} -flto -T ${CMAKE_SOURCE_DIR}/chip/${CHIP}/${BOOT_START_ADDR}/link.ld -Wl,-Map=boot.map")

set(TARGET boot)

add_executable(${TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#if (${BOOT_TYPE} MATCHES flash)
#    target_sources(${TARGET} PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/bootflash.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/bootlcd.cpp
#    )
#elseif (${BOOT_TYPE} MATCHES uart)
#    target_sources(${TARGET} PRIVATE
#        ${CMAKE_CURRENT_SOURCE_DIR}/bootuart.cpp
#    )
#endif()

target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bootaction/${BOOT_TYPE}/bootaction.cpp
)

#target_include_directories(${TARGET} PRIVATE 
#    ${CMAKE_CURRENT_SOURCE_DIR}
#    ${CMAKE_SOURCE_DIR}/lib
#)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")

target_link_libraries(${TARGET} c_nano)

add_subdirectory(drivers)
add_subdirectory(lib)

size(${TARGET})
generate_bin(${TARGET})
flash(${TARGET} ${BOOT_START_ADDR})
