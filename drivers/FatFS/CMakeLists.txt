target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/diskio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ff.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ffsystem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ffunicode.c
)

target_include_directories(drivers PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/logic/config
)