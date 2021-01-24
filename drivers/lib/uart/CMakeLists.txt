add_subdirectory(sensors)

if (USE_LOG)
    add_subdirectory(log)
endif()

if (USE_GSM)
    add_subdirectory(GSM)    
endif()
