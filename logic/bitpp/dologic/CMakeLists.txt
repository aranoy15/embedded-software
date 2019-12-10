target_sources(${TARGET} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/commandhandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lcdtask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sensortask.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/archivetask.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/blinktask.cpp
)

target_include_directories(${TARGET} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
