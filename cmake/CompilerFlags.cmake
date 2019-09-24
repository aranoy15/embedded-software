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

if (DEBUG)
    SET(OPT_FLAGS "-g")
else()
    SET(OPT_FLAGS "-Os")
endif()

SET(SPECS "-specs=nosys.specs -specs=nano.specs") 
SET(SECTIONS "-ffunction-sections -fdata-sections")
SET(SHARE_SETTINGS "-fno-builtin -fomit-frame-pointer -fno-unroll-loops -ffast-math -ftree-vectorize")

SET(CMAKE_C_FLAGS "${PLATFORM_FLAGS} ${SECTIONS} ${SHARE_SETTINGS} ${OPT_FLAGS} -Wall -std=gnu99")
SET(CMAKE_CXX_FLAGS "${PLATFORM_FLAGS} ${SECTIONS} ${SHARE_SETTINGS} ${OPT_FLAGS} -Weffc++ -std=c++2a -fno-exceptions")
SET(CMAKE_ASM_FLAGS "${PLATFORM_FLAGS} ${OPT_FLAGS} -x assembler-with-cpp")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections ${PLATFORM_FLAGS} ${SPECS} -T ${CMAKE_CURRENT_SOURCE_DIR}/chip/${CHIP}/${APP_START_ADDR}/link.ld")
SET(CMAKE_MODULE_LINKER_FLAGS "${PLATFORM_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${PLATFORM_FLAGS}")

set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)

set(CMAKE_C_LINK_FLAGS "")
set(CMAKE_CXX_LINK_FLAGS "")
set(CMAKE_OSX_SYSROOT "")

add_definitions(-D__FILENAME__="")