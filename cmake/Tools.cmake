function (add_trace NAME)
	add_custom_target(${NAME}.trace
		arm-none-eabi-gdb-py --batch --command=${CMAKE_SOURCE_DIR}/debug/trace_gdb.py --args ${CMAKE_CURRENT_BINARY_DIR}/logic/${LOGIC}/${NAME}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		USES_TERMINAL
		)
endfunction ()

function (flash NAME)
	add_custom_target(${NAME}.flash
		st-flash write ${CMAKE_BINARY_DIR}/${NAME}.bin 0x8000000
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		USES_TERMINAL
		)
endfunction ()

function(size NAME)
	add_custom_target(size ALL
		arm-none-eabi-size ${CMAKE_BINARY_DIR}/logic/${LOGIC}/${NAME}
		DEPENDS ${NAME}
	)	
endfunction(size NAME)

function(read)
	if (NOT ${PORT} STREQUAL "")
		add_custom_target(read
			python3 ${CMAKE_SOURCE_DIR}/pyutils/reader.py -b 115200 ${PORT}
		)	
		message("Port is defined. Port is ${PORT}")
	else ()
		message("Port is not defined. Function unavailable.")
	endif ()
endfunction(read)

function(generate_bin NAME)
	add_custom_target(generate_bin ALL
		arm-none-eabi-objcopy -Obinary ${CMAKE_BINARY_DIR}/logic/${LOGIC}/${NAME} ${CMAKE_BINARY_DIR}/${NAME}.bin
		DEPENDS ${NAME}
	)	
	add_custom_target(generate_hex ALL
		arm-none-eabi-objcopy -Oihex ${CMAKE_BINARY_DIR}/logic/${LOGIC}/${NAME} ${CMAKE_BINARY_DIR}/${NAME}.hex
		DEPENDS ${NAME}
	)	
endfunction(generate_bin NAME)

add_custom_target(openocd
openocd -f /usr/local/Cellar/open-ocd/0.10.0/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/local/Cellar/open-ocd/0.10.0/share/openocd/scripts/target/stm32f1x.cfg
	USES_TERMINAL
)