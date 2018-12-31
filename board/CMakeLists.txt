set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \
		-T ${CMAKE_CURRENT_SOURCE_DIR}/board/${BOARD}/link.ld \
		-Xlinker -Map=${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.map \
        -Xlinker --no-wchar-size-warning")

add_subdirectory(${BOARD})