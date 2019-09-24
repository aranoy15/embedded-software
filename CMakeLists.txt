cmake_minimum_required(VERSION 3.6)

set(DEBUG 0)

add_definitions(-DDEBUG=${DEBUG})

project(test C CXX ASM)

if (NOT DEFINED LOGIC)
    message(FATAL_ERROR "-- Logic is not defined")
endif()

if (NOT DEFINED BOARD)
    message(FATAL_ERROR "-- Board is not defined")
endif()

set(COMPILER GCC)

set(TARGET app)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(Board/${BOARD})
include(Logic/${LOGIC})
include(CompilerFlags)
include(Tools)

add_subdirectory(app)
add_subdirectory(chip)
add_subdirectory(drivers)
add_subdirectory(lib)

size(${TARGET})
generate_bin(${TARGET})

flash(${TARGET} ${APP_START_ADDR})

add_trace(${TARGET})
read()
upload()

add_subdirectory(boot)