ifeq ($(OS),Windows_NT)
	ARCH=win32
	CFLAGS=/I"../arch-win32" /I../common /I../freetype-2.4.10/include /Zi /nologo /W4 /WX /Od /Oy- /D WIN32 /D _DEBUG /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _UNICODE /D UNICODE /Gm- /EHsc /RTC1 /MTd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Fo"Debug\\" /Fd"Debug\vc100.pdb" /c
	LINKING=${LINK} /OUT:"frustrum.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST:NO /ALLOWISOLATION:NO /DEBUG /PDB:"frustrum.pdb" /SUBSYSTEM:WINDOWS /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"frustrum.lib" /MACHINE:X86 /LIBPATH:$(LIB1) /LIBPATH:$(LIB2) $(OBJS) $(LIBS) /NODEFAULTLIB:MSVCRTD.lib
	LIBS=winmm.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib
else
	CFLAGS=-I../arch-linux -I../common -I/usr/include/freetype2 -g -c
	ARCH=linux
	LINKING=$(CC) -g -o $@ $^ -lrt -lGL -lpng -lfreetype -lglut -lX11 -lEGL -lGLU
endif 

all: frustrum.exe pixmap.exe pictest.exe

frustrum.exe: frustrum.obj app-$(ARCH).obj app.obj
	$(LINKING)

pixmap.exe: pixmap.obj app-$(ARCH).obj app.obj
	$(LINKING)

pictest.exe: pictest.obj pic.obj app-$(ARCH).obj app.obj cart.png.obj
	$(LINKING)

pictest.c : ../common/pic.c ../common/app-$(ARCH).c

%.obj : %.c
	${CC} ${CFLAGS} $< -o $@

%.obj : ../common/%.c
	${CC} ${CFLAGS} $< -o $@
	
clean:
	rm -f *.exe *.obj
