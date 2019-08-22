target_sources(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lcdlogic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/clocklogic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/sensorlogic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ledlogic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/datastorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logicstate.cpp
)

target_include_directories(app PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)