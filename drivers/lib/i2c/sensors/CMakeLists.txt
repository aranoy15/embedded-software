if (USE_BME280)
target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bme280.cpp
)
endif()