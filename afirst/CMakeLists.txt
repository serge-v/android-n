cmake_minimum_required(VERSION 2.8)

include_directories(../common)
include_directories(../arch-osx)
include_directories(/usr/local/include/freetype2)
include_directories(/usr/local/include)

find_library(ft_LIBRARY freetype)
find_library(png_LIBRARY png)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework GLUT -framework OpenGL")

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

add_executable(
	afirst
	jni/main.c
	jni/font.c
	jni/game.c
	../common/pic.c
	jni/pumpkin.c
	jni/worm.c
	jni/cart.c
	jni/leika.c
	jni/spray.c
	jni/resources.c
	jni/sound-osx.c
	../common/app-osx.c
	../common/app.c
	${RESOURCES}
)

target_link_libraries(
	afirst
	${ft_LIBRARY}
	${png_LIBRARY}
)
