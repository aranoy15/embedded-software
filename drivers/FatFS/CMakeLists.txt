add_library(FatFS 
    #${CMAKE_CURRENT_SOURCE_DIR}/diskio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ff.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ffsystem.c
)

target_include_directories(FatFS PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/logic/config
)