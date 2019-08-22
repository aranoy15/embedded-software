#add_library(bsp 
#    ${CMAKE_CURRENT_SOURCE_DIR}/ST/bsp_template.cpp
#)

target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ST/bsp_template.cpp
)

target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ST
)

#include_directories(${BOARD})
add_subdirectory(${BOARD})

#target_link_libraries(bsp PRIVATE csp)
