ifeq ($(OS),Windows_NT)
	ARCH=win32
	CFLAGS=/I"../arch-win32" /I../common /I../freetype-2.4.10/include /Zi /nologo /W4 /WX /Od /Oy- /D WIN32 /D _DEBUG /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _UNICODE /D UNICODE /Gm- /EHsc /RTC1 /MTd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Fo"Debug\\" /Fd"Debug\vc100.pdb" /c
	LINKING=${LINK} /OUT:"Debug\afirst.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST:NO /ALLOWISOLATION:NO /DEBUG /PDB:"Debug\afirst.pdb" /SUBSYSTEM:WINDOWS /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"Debug\afirst.lib" /MACHINE:X86 /LIBPATH:$(LIB1) /LIBPATH:$(LIB2) $(OBJS) $(LIBS) /NODEFAULTLIB:MSVCRTD.lib
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		CFLAGS=-Wno-deprecated-declarations -I../arch-osx -I../common -I/usr/local/include/freetype2 -g -c
		ARCH=osx
		LINKING=$(CC) -g -o debug/afirst $(OBJS) -lfreetype -lpng -framework GLUT -framework OpenGL
	else
		CFLAGS=-I../arch-linux -I../common -I/usr/include/freetype2 -g -c
		ARCH=linux
		LINKING=$(CC) -g -o debug/afirst.exe $(OBJS) -lrt -lGL -lpng -lfreetype -lglut -lX11 -lEGL
	endif
endif 

LIBS=winmm.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib \
../freetype-2.4.10/objs/win32/vc2010/freetype2410MT_D.lib \
../libpng/projects/vstudio/Debug\ Library/libpng15.lib \
../libpng/projects/vstudio/Debug\ Library/zlib.lib

OBJS=\
    debug/pumpkin_red.png.obj     \
    debug/courier.png.obj         \
    debug/button_test.png.obj     \
    debug/button_quit.png.obj     \
    debug/font.png.obj            \
    debug/pumpkin_green.png.obj   \
    debug/pumpkin_yellow.png.obj  \
    debug/pumpkin_mask.png.obj    \
    debug/pumpkin_spoiled.png.obj \
    debug/bg.png.obj              \
    debug/cart.png.obj            \
    debug/cart_mask.png.obj       \
    debug/worm1.png.obj           \
    debug/worm1_mask.png.obj      \
    debug/worm2.png.obj           \
    debug/worm2_mask.png.obj      \
    debug/worm3.png.obj           \
    debug/worm3_mask.png.obj      \
    debug/leika.png.obj           \
    debug/leika_mask.png.obj      \
    debug/spray.png.obj           \
    debug/spray_mask.png.obj      \
    debug/main.obj              \
    debug/font.obj              \
    debug/game.obj              \
    debug/pic.obj               \
    debug/pumpkin.obj           \
    debug/worm.obj              \
    debug/cart.obj              \
    debug/leika.obj             \
    debug/spray.obj             \
    debug/resources.obj         \
    debug/bg.wav.obj            \
    debug/sound-$(ARCH).obj     \
    debug/app-$(ARCH).obj       \
    debug/app.obj

all: afirst-$(ARCH)

# ========================================================================================

#bmpconv: ../bmpconv/debug/bmpconv.exe

a: afirst-$(ARCH) afirst-android-debug

ar: afirst-android-release

afirst-android-debug: bin/afirst-debug.apk

afirst-android-release: bin/afirst.apk /cygdrive/z/afirst.apk

afirst-$(ARCH): debug/afirst.exe

# ========================================================================================

bin/afirst-debug.apk: jni/*.c
	ndk
	a

bin/afirst.apk : bin/afirst-release-unsigned.apk
	sign afirst

/cygdrive/z/afirst.apk: bin/afirst.apk
	cp bin/afirst.apk /cygdrive/z/afirst.apk

bin/afirst-release-unsigned.apk: jni/*.c
	ndk
	ant release

debug/mkresource: mkresource.c
	gcc $< -o $@

debug/afirst.exe: $(OBJS)
	$(LINKING)

debug/%.obj : jni/%.c
	${CC} ${CFLAGS} $< -o $@

debug/%.obj : ../common/%.c
	${CC} ${CFLAGS} $< -o $@
	
debug/%.wav.obj : jni/gen/%.wav.c
	${CC} ${CFLAGS} $< -o $@

debug/%.png.obj : jni/gen/%.png.c
	${CC} ${CFLAGS} $< -o $@

.PRECIOUS: jni/gen/%.wav.c jni/gen/%.png.c

jni/gen/%.png.c : med/%.xcf debug/mkresource
	convert $< -depth 24 jni/gen/$(basename $(notdir $<)).png
	debug/mkresource jni/gen/$(basename $(notdir $<)).png $(basename $(notdir $<))_png \
		jni/gen/$(basename $(notdir $<)).png.h jni/gen/$(basename $(notdir $<)).png.c

jni/gen/%.wav.c : med/%.wav debug/mkresource
	sox $< -r 8k -c 1 jni/gen/$(basename $(notdir $<)).s16
	debug/mkresource jni/gen/$(basename $(notdir $<)).s16 $(basename $(notdir $<))_s16 \
		jni/gen/$(basename $(notdir $<)).wav.h jni/gen/$(basename $(notdir $<)).wav.c

depends: jni/*.c
	gcc -I../arch-win32 -I../common /I../freetype-2.4.10/include -MM $^ | sed 's/\(.*\.o\)/debug\/\1bj/' > depends.txt

-include depends.txt

clean:
	rm -f debug/*.* jni/gen/*.*
	rm -rf libs bin obj
