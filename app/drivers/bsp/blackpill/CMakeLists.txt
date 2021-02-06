target_sources(drivers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp.cpp
)

if (USE_FREERTOS)
    target_sources(drivers PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/bspMem.cpp
    )
endif()

target_include_directories(drivers PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)