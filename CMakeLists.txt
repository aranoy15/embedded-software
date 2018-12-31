cmake_minimum_required(VERSION 3.5)

option(DBG "Enable or disable debugging" ON)

if (NOT DEFINED BOARD)
    set(BOARD "STM32F103C8T6")
endif()

if (NOT DEFINED LOGIC)
    set(LOGIC "main")
endif()

if (NOT DEFINED FAMILY)
    set(FAMILY "STM32F1")
endif()

if (NOT DEFINED TARGET)
    set(TARGET "main")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(CompilerFlags)
include(Tools)

add_subdirectory(board)
add_subdirectory(drivers)
add_subdirectory(logic)