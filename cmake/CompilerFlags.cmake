# specify the cross compiler
enable_language(ASM)

#if (APPLE)
#    SET(TOOLCHAIN_PREFIX "~/Documents/Toolchains/gcc-arm-none-eabi/")
#endif()

SET(TARGET_TRIPLET "arm-none-eabi")

SET(CMAKE_C_COMPILER "arm-none-eabi-gcc")
SET(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
SET(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_AR "arm-none-eabi-gcc-ar")
set(CMAKE_LINKER "arm-none-eabi-gcc-ld")
set(CMAKE_RANLIB "arm-none-eabi-ranlib")

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(CMAKE_C_FLAGS "-mthumb -g -fno-builtin -mcpu=cortex-m3 -Wall -std=gnu99 -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-math -ftree-vectorize" CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "-mthumb -g -Weffc++ -fno-exceptions -fno-builtin -mcpu=cortex-m3 -Wall -std=c++11 -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-math -ftree-vectorize" CACHE INTERNAL "cxx compiler flags")
SET(CMAKE_ASM_FLAGS "-mthumb -g -mcpu=cortex-m3 -x assembler-with-cpp")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -mthumb -mcpu=cortex-m3 -mabi=aapcs -specs=nosys.specs -specs=nano.specs -T ${CMAKE_CURRENT_SOURCE_DIR}/board/${CHIP}/link.ld")
SET(CMAKE_MODULE_LINKER_FLAGS "-mthumb -mcpu=cortex-m3 -mabi=aapcs")
SET(CMAKE_SHARED_LINKER_FLAGS "-mthumb -mcpu=cortex-m3 -mabi=aapcs")

set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)

set(CMAKE_C_LINK_FLAGS "")
set(CMAKE_CXX_LINK_FLAGS "")
set(CMAKE_OSX_SYSROOT "")

add_definitions(-D__FILENAME__="")