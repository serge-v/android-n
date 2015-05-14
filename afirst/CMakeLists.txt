cmake_minimum_required(VERSION 2.8)

include_directories(../common)

if(ANDROID)

	include_directories(/opt/android-ndk-r10d/sources/android/native_app_glue)
	include_directories(/opt/freetype/include/freetype2)
	include_directories(/opt/libpng/include)

	find_library(ft_LIBRARY freetype PATHS /opt/freetype/lib NO_DEFAULT_PATH)
	find_library(png_LIBRARY png PATHS /opt/libpng/lib NO_DEFAULT_PATH)
	message("freetype:" ${ft_LIBRARY})
	message("libpng:" ${png_LIBRARY})

	list(APPEND SOURCES /opt/android-ndk-r10d/sources/android/native_app_glue/android_native_app_glue.c)
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

list(APPEND RESOURCES
	jni/gen/pumpkin_red.png.c
	jni/gen/courier.png.c
	jni/gen/button_test.png.c
	jni/gen/button_quit.png.c
	jni/gen/font.png.c
	jni/gen/pumpkin_green.png.c
	jni/gen/pumpkin_yellow.png.c
	jni/gen/pumpkin_mask.png.c
	jni/gen/pumpkin_spoiled.png.c
	jni/gen/bg.png.c
	jni/gen/cart.png.c
	jni/gen/cart_mask.png.c
	jni/gen/worm1.png.c
	jni/gen/worm1_mask.png.c
	jni/gen/worm2.png.c
	jni/gen/worm2_mask.png.c
	jni/gen/worm3.png.c
	jni/gen/worm3_mask.png.c
	jni/gen/leika.png.c
	jni/gen/leika_mask.png.c
	jni/gen/spray.png.c
	jni/gen/spray_mask.png.c
	jni/gen/bg.wav.c
)

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
	../common/audio.c
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

source_group(resources FILES ${RESOURCES})
source_group(sources FILES ${SOURCES})
source_group(headers FILES ${HEADERS})

add_library(
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
	EGL
	GLESv1_CM
	OpenSLES
	android
)

if(ANDROID)
	execute_process(COMMAND /opt/android-sdk/tools/android create project
		--path ${CMAKE_CURRENT_BINARY_DIR}/android
		--target android-15
		--name afirst
		--package mila.afirst
		--activity DummyActivity
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

	set(ANDROID_SO_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/android/libs/armeabi)
	set_target_properties(afirst PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${ANDROID_SO_OUTDIR})

	add_custom_command(
		TARGET afirst
		POST_BUILD
		COMMAND echo edit config
		COMMAND ant debug
		ARGS -quiet
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/android
		COMMENT "Creating apk"
	)

endif()











