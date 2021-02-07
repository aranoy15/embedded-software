cmake_minimum_required(VERSION 3.6)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR ARM)

project(test C CXX ASM)


if (NOT DEFINED LOGIC)
    message(FATAL_ERROR "-- Logic is not defined")
endif()

if (NOT DEFINED BOARD)
    message(FATAL_ERROR "-- Board is not defined")
endif()

if (NOT DEFINED DEBUG)
    set(DEBUG 0)
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

target_include_directories(${TARGET} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

size(${TARGET})
generate_bin(${TARGET})

flash(${TARGET} ${APP_START_ADDR})

read()
upload()

if (HAVE_BOOT)
    add_subdirectory(boot)
endif()
