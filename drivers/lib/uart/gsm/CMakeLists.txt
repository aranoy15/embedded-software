
if (USE_SIM900)
    target_sources(${TARGET} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/gsm.cpp
    )
endif()