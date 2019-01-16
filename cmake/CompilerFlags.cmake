# specify the cross compiler
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)

set(CMAKE_ASM-ATT_COMPILER arm-none-eabi-gcc)
enable_language(ASM-ATT)
set(CMAKE_ASM-ATT_COMPILER arm-none-eabi-gcc)

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#set(COMMON_FLAGS "-W -Wall -fdiagnostics-color=always")

if (DBG)
    set(COMMON_FLAGS "-g ${COMMON_FLAGS}")
endif ()

add_definitions(-DUSE_HAL_DRIVER)

set(OPT_FLAGS "-Os -ffunction-sections -fdata-sections")
set(PLATFORM_FLAGS "-mcpu=cortex-m3 -mthumb")
set(CMAKE_CXX_FLAGS "-Weffc++ -fno-exceptions ${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} ${OPT_FLAGS} ${PLATFORM_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} ${OPT_FLAGS} ${PLATFORM_FLAGS}")
set(CMAKE_ASM-ATT_FLAGS "${CMAKE_ASM-ATT_FLAGS} -c ${COMMON_FLAGS} ${PLATFORM_FLAGS}")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${COMMON_FLAGS} ${PLATFORM_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=nosys.specs -lnosys -lc -lm -Xlinker --gc-sections")