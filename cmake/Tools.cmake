function (add_trace NAME)
	add_custom_target(${NAME}.trace
		arm-none-eabi-gdb-py --batch --command=${CMAKE_SOURCE_DIR}/debug/trace_gdb.py --args ${CMAKE_CURRENT_BINARY_DIR}/${NAME}/${NAME}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		USES_TERMINAL
		)
endfunction ()

function (flash NAME ADDRESS)
    if (MSVC OR MSYS OR MINGW) 
        add_custom_target(${NAME}.flash
	    	ST-LINK_CLI -P ${CMAKE_BINARY_DIR}/${NAME}.bin ${ADDRESS}
	    	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	    	USES_TERMINAL
            )
    else()
	    add_custom_target(${NAME}.flash
	    	st-flash write ${CMAKE_BINARY_DIR}/${NAME}.bin ${ADDRESS}
	    	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	    	USES_TERMINAL
            )
    endif()
endfunction ()

function(size NAME)
	add_custom_target(${NAME}.size ALL
		arm-none-eabi-size ${CMAKE_BINARY_DIR}/${NAME}/${NAME}
		DEPENDS ${NAME}
	)	
endfunction(size NAME)

function(read)
	if (NOT ${PORT} STREQUAL "")
		add_custom_target(read
			${CMAKE_SOURCE_DIR}/qtutil/release/reader -c ${PORT} -b 115200
		)	
		message("Port is defined. Port is ${PORT}")
	else ()
		message("Port is not defined. Function unavailable.")
	endif ()
endfunction(read)

function(upload)
	if (NOT ${PORT} STREQUAL "")
		add_custom_target(upload
			${CMAKE_SOURCE_DIR}/qtutil/release/uploader -c ${PORT} -b 115200 -f ${CMAKE_BINARY_DIR}/app.bin #&& ${CMAKE_SOURCE_DIR}/qtutil/release/reader -c ${PORT} -b 115200
		)	
		message("Port is defined. Port is ${PORT}")
	else ()
		message("Port is not defined. Function unavailable.")
	endif ()
endfunction(upload)

function(generate_bin NAME)
	add_custom_target(${NAME}.generate_bin ALL
		arm-none-eabi-objcopy -Obinary ${CMAKE_BINARY_DIR}/${NAME}/${NAME} ${CMAKE_BINARY_DIR}/${NAME}.bin
		DEPENDS ${NAME}
	)	
	add_custom_target(${NAME}.generate_hex ALL
		arm-none-eabi-objcopy -Oihex ${CMAKE_BINARY_DIR}/${NAME}/${NAME} ${CMAKE_BINARY_DIR}/${NAME}.hex
		DEPENDS ${NAME}
	)	
endfunction(generate_bin NAME)

add_custom_target(openocd
openocd -f /usr/local/Cellar/open-ocd/0.10.0/share/openocd/scripts/interface/stlink-v2.cfg -f /usr/local/Cellar/open-ocd/0.10.0/share/openocd/scripts/target/stm32f1x.cfg
	USES_TERMINAL
)