target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_core.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_ctlreq.c
    ${CMAKE_CURRENT_SOURCE_DIR}/usbd_ioreq.c
)