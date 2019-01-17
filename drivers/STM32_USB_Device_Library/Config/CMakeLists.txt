target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_conf.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_desc.c
)