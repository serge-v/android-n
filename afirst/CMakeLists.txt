cmake_minimum_required(VERSION 2.8)

include_directories(../common)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(ANDROID)

	include_directories(/opt/android-ndk-r10d/sources/android/native_app_glue)
	include_directories(/opt/freetype/include/freetype2)
	include_directories(/opt/libpng/include)

	find_library(ft_LIBRARY libfreetype.a PATHS /opt/freetype/lib NO_DEFAULT_PATH)
	find_library(png_LIBRARY libpng.a PATHS /opt/libpng/lib NO_DEFAULT_PATH)
	message("freetype:" ${ft_LIBRARY})
	message("libpng:" ${png_LIBRARY})

	list(APPEND SOURCES /opt/android-ndk-r10d/sources/android/native_app_glue/android_native_app_glue.c)
	list(APPEND SOURCES ../common/audio.c)
	list(APPEND LIBS android log EGL OpenSLES GLESv2 GLESv1_CM z m)

else()
	set(ARCH osx)
	include_directories(/usr/local/include/freetype2)
	include_directories(/usr/local/include)
	include_directories(../arch-osx)

	find_library(ft_LIBRARY freetype)
	find_library(png_LIBRARY png)

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework GLUT -framework OpenGL")

endif()

message("ARCH:" ${ARCH})

macro(CONV_RESOURCE SRC HDR SUFFIX file)
	set(${SRC})
	set(${HDR})

	get_filename_component(basename "${file}" NAME_WE)

	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gen)
	set(rsfile ${CMAKE_CURRENT_BINARY_DIR}/gen/${basename}.${SUFFIX})
	set(hsfile ${CMAKE_CURRENT_BINARY_DIR}/gen/${basename}.${SUFFIX}.h)
	set(csfile ${CMAKE_CURRENT_BINARY_DIR}/gen/${basename}.${SUFFIX}.c)

	if(${SUFFIX} STREQUAL "png")
		set(converter convert)
		set(converter_args "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "-depth" "24" "${rsfile}")
	elseif(${SUFFIX} STREQUAL "s16")
		set(converter "sox")
		set(converter_args "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "-r" "8k" "-c" "1" "${rsfile}")
	else()
		message(FATAL_ERROR "invalid SUFFIX parameter")
	endif()

	add_custom_command(
		OUTPUT "${csfile}" "${hsfile}" "${rsfile}"
		COMMAND ${converter}
		ARGS ${converter_args}
		COMMAND mkresource
		ARGS ${rsfile} ${basename}_${SUFFIX} ${hsfile} ${csfile}
		DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
		COMMENT "Generating C code from ${file}"
	)

	list(APPEND ${SRC} ${csfile})
	list(APPEND ${HDR} ${hsfile})
	
	list(APPEND CLEAN ${rsfile} ${csfile} ${hsfile})

	set_directory_properties(ADDITIONAL_MAKE_CLEAN_FILES ${${CLEAN}})
	set_source_files_properties(${${CLEAN}} PROPERTIES GENERATED TRUE)
endmacro(CONV_RESOURCE SRC HDR SUFFIX file)

list(APPEND PICTURES
	med/pumpkin_red.xcf
	med/courier.xcf
	med/button_test.xcf
	med/button_quit.xcf
	med/font.xcf
	med/pumpkin_green.xcf
	med/pumpkin_yellow.xcf
	med/pumpkin_mask.xcf
	med/pumpkin_spoiled.xcf
	med/bg.xcf
	med/cart.xcf
	med/cart_mask.xcf
	med/worm1.xcf
	med/worm1_mask.xcf
	med/worm2.xcf
	med/worm2_mask.xcf
	med/worm3.xcf
	med/worm3_mask.xcf
	med/leika.xcf
	med/leika_mask.xcf
	med/spray.xcf
	med/spray_mask.xcf
)

list(APPEND SOUNDS
	med/bg.wav
)

foreach(file ${PICTURES})
	conv_resource(SRC HDR "png" ${file})
	list(APPEND GENERATED ${SRC} ${HDR})
endforeach()

foreach(file ${SOUNDS})
	conv_resource(SRC HDR "s16" ${file})
	list(APPEND GENERATED ${SRC} ${HDR})
endforeach()

list(APPEND SOURCES
	jni/main.c
	jni/font.c
	jni/game.c
	jni/pumpkin.c
	jni/worm.c
	jni/cart.c
	jni/leika.c
	jni/spray.c
	jni/resources.c
	jni/sound-${ARCH}.c
	../common/app-${ARCH}.c
	../common/app.c
	../common/pic.c
	${GENERATED}
)

list(APPEND HEADERS
	jni/game.h
	jni/pumpkin.h
	jni/worm.h
	jni/cart.h
	jni/leika.h
	jni/spray.h
	jni/resources.h
	../common/pic.h
	../common/app.h
)

source_group(resources FILES ${XCF_RESOURCES})
source_group(resources FILES ${WAV_RESOURCES})
source_group(sources FILES ${SOURCES})
source_group(headers FILES ${HEADERS})

if(ANDROID)
	add_library(
#	add_executable(
		afirst
		SHARED
		${SOURCES}
		${HEADERS}
		${RESOURCES}
	)

	target_link_libraries(
		afirst
		${ft_LIBRARY}
		${png_LIBRARY}
		${LIBS}
	)

	execute_process(COMMAND /opt/android-sdk/tools/android create project
		--path ${CMAKE_CURRENT_BINARY_DIR}/android
		--target android-15
		--name afirst
		--package mila.afirst
		--activity DummyActivity
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

	file(COPY AndroidManifest.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/android/)

	set(ANDROID_SO_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/android/libs/armeabi)
	set_target_properties(afirst PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${ANDROID_SO_OUTDIR})

	add_custom_command(
		TARGET afirst
		POST_BUILD
		COMMAND ant debug
		ARGS -quiet
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/android
		COMMENT "Creating apk"
	)
	
	add_executable(hellow hellow.c)
else()
	add_executable(
		afirst
		${SOURCES}
		${HEADERS}
		${RESOURCES}
	)

	target_link_libraries(
		afirst
		${ft_LIBRARY}
		${png_LIBRARY}
	)

	add_executable(mkresource mkresource.c)
	add_dependencies(afirst mkresource)
endif()


