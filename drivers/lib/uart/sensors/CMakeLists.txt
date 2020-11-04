if (USE_MHZ19)

target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/mhz19.cpp
)

endif()