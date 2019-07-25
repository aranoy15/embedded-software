cmake_minimum_required(VERSION 3.6)

set(DEBUG 0)

project(test C CXX ASM)

if (NOT DEFINED CHIP)
    set(CHIP STM32F103xB)
endif()

if (NOT DEFINED LOGIC)
    set(LOGIC main)
endif()

if (NOT DEFINED FAMILY)
    set(FAMILY F1)
endif()

if (NOT DEFINED TARGET)
    set(TARGET app)
endif()

add_definitions(-D${CHIP})
set(HAVE_FREERTOS ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#include(Setting)
include(CompilerFlags)
include(Tools)

add_subdirectory(app)
add_subdirectory(board)
add_subdirectory(drivers)
add_subdirectory(lib)

size(${TARGET})
generate_bin(${TARGET})
flash(${TARGET} 0x8000000)
add_trace(${TARGET})
read()

add_subdirectory(boot)