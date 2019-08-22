#add_library(csp 
#    ${CMAKE_CURRENT_SOURCE_DIR}/csp.cpp
#)

target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/csp.cpp
)

target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

#target_link_libraries(csp PRIVATE drivers)