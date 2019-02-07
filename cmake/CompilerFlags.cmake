# specify the cross compiler
enable_language(ASM)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

SET(CMAKE_C_FLAGS "-mthumb -g -fno-builtin -mcpu=cortex-m3 -Wall -std=gnu99 -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-math -ftree-vectorize" CACHE INTERNAL "c compiler flags")
SET(CMAKE_CXX_FLAGS "-mthumb -g -Weffc++ -fno-exceptions -fno-builtin -mcpu=cortex-m3 -Wall -std=c++11 -ffunction-sections -fdata-sections -fomit-frame-pointer -mabi=aapcs -fno-unroll-loops -ffast-math -ftree-vectorize" CACHE INTERNAL "cxx compiler flags")
SET(CMAKE_ASM_FLAGS "-mthumb -g -mcpu=cortex-m3 -x assembler-with-cpp")

SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections -mthumb -mcpu=cortex-m3 -mabi=aapcs -T ${CMAKE_CURRENT_SOURCE_DIR}/board/${CHIP}/link.ld")
SET(CMAKE_MODULE_LINKER_FLAGS "-mthumb -mcpu=cortex-m3 -mabi=aapcs")
SET(CMAKE_SHARED_LINKER_FLAGS "-mthumb -mcpu=cortex-m3 -mabi=aapcs")

add_definitions(-D__FILENAME__="")