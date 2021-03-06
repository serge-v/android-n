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
	list(APPEND LIBS ui android log EGL OpenSLES GLESv2 GLESv1_CM z m)

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
		set(converter /usr/local/bin/convert)
		set(converter_args "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "-depth" "24" "${rsfile}")
	elseif(${SUFFIX} STREQUAL "s16")
		set(converter "/usr/local/bin/sox")
		set(converter_args "${CMAKE_CURRENT_SOURCE_DIR}/${file}" "-r" "8k" "-c" "1" "${rsfile}")
	else()
		message(FATAL_ERROR "invalid SUFFIX parameter")
	endif()

	add_custom_command(
		OUTPUT "${csfile}" "${hsfile}" "${rsfile}"
		COMMAND ${converter}
		ARGS ${converter_args}
		COMMAND $ENV{HOME}/bin/mkresource
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

macro(GEN_PICS SRCS)
	set(${SRCS})

	list(APPEND OUT_SOURCES
		${CMAKE_CURRENT_BINARY_DIR}/gen/oxygen.png.c
		${CMAKE_CURRENT_BINARY_DIR}/gen/oxygen_mask.png.c
		${CMAKE_CURRENT_BINARY_DIR}/gen/carbon.png.c
		${CMAKE_CURRENT_BINARY_DIR}/gen/carbon_mask.png.c
	)

	add_custom_command(
		OUTPUT ${OUT_SOURCES}
		COMMAND ${CMAKE_SOURCE_DIR}/gen-pics.sh
		DEPENDS ${CMAKE_SOURCE_DIR}/gen-pics.sh
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gen
		COMMENT "Generating imagemagick for pngs and mkresource for C code"
	)

	list(APPEND ${SRCS} ${OUT_SOURCES})
	set_source_files_properties(
		${${OUT_SOURCES}}
		${CMAKE_CURRENT_BINARY_DIR}/gen/oxygen.png
		${CMAKE_CURRENT_BINARY_DIR}/gen/oxygen_mask.png
		${CMAKE_CURRENT_BINARY_DIR}/gen/oxygen.png.h
		${CMAKE_CURRENT_BINARY_DIR}/gen/oxygen_mask.png.h
		${CMAKE_CURRENT_BINARY_DIR}/gen/carbon.png
		${CMAKE_CURRENT_BINARY_DIR}/gen/carbon_mask.png
		${CMAKE_CURRENT_BINARY_DIR}/gen/carbon.png.h
		${CMAKE_CURRENT_BINARY_DIR}/gen/carbon_mask.png.h
		PROPERTIES GENERATED TRUE
	)
endmacro(GEN_PICS SRCS)

gen_pics(SRCS)
list(APPEND GENERATED ${SRCS})
message("generated:" ${GENERATED})

list(APPEND PICTURES
	med/courier.xcf
	med/button_test.xcf
	med/button_quit.xcf
	med/font.xcf
	med/bg.xcf
	med/hidrogen.xcf
	med/hidrogen_mask.xcf
	med/bond1.xcf
	med/bond2.xcf
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
	jni/spray.c
	jni/molecule.c
	jni/resources.c
	jni/sound-${ARCH}.c
	../common/app-${ARCH}.c
	../common/app.c
	../common/pic.c
)

list(APPEND HEADERS
	jni/game.h
	jni/spray.h
	jni/molecule.h
	jni/resources.h
	../common/pic.h
	../common/app.h
)

source_group(resources FILES ${PICTURES})
source_group(sources FILES ${SOURCES})
source_group(headers FILES ${HEADERS})
source_group(generated FILES ${GENERATED})

if(ANDROID)
	add_executable(
		molecules-check-symbols
		jni/checker.c
		${SOURCES}
		${HEADERS}
		${GENERATED}
	)

	target_link_libraries(
		molecules-check-symbols
		${ft_LIBRARY}
		${png_LIBRARY}
		${LIBS}
	)

	add_library(
		molecules
		SHARED
		${SOURCES}
		${HEADERS}
		${GENERATED}
	)

	target_link_libraries(
		molecules
		${ft_LIBRARY}
		${png_LIBRARY}
		${LIBS}
	)

	execute_process(COMMAND /opt/android-sdk/tools/android create project
		--path ${CMAKE_CURRENT_BINARY_DIR}/android
		--target android-15
		--name molecules
		--package milla.molecules
		--activity DummyActivity
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

	file(COPY AndroidManifest.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/android/)

	set(ANDROID_SO_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/android/libs/armeabi)
	set_target_properties(molecules PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${ANDROID_SO_OUTDIR})

	add_custom_command(
		TARGET molecules
		POST_BUILD
		COMMAND ant debug
		ARGS -quiet
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/android
		COMMENT "Creating apk"
	)
else()
	add_executable(
		molecules
		${SOURCES}
		${HEADERS}
		${GENERATED}
	)

	target_link_libraries(
		molecules
		${ft_LIBRARY}
		${png_LIBRARY}
	)
endif()


