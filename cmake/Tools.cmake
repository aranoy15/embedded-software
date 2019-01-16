function (add_trace NAME)
	add_custom_target(${NAME}.trace
		arm-none-eabi-gdb --batch --command=${CMAKE_SOURCE_DIR}/debug/trace_gdb.py --args ${CMAKE_CURRENT_BINARY_DIR}/${NAME}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		USES_TERMINAL
		)
endfunction ()

function (flash NAME)
	add_custom_target(flash
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


add_custom_target(openocd
openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/share/openocd/scripts/target/stm32f1x.cfg
	USES_TERMINAL
)