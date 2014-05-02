OBJS=debug/bmpconv.obj

all: bmpconv-win32

bmpconv-win32: debug/bmpconv.exe

debug/bmpconv.exe: $(SNDS) $(RGBAS) $(OBJS)
	${LINK} /OUT:"Debug\bmpconv.exe" /INCREMENTAL:NO /NOLOGO /MANIFEST:NO /ALLOWISOLATION:NO /DEBUG /PDB:"Debug\bmpconv.pdb" /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT /IMPLIB:"Debug\bmpconv.lib" /MACHINE:X86 /LIBPATH:$(LIB1) /LIBPATH:$(LIB2) $(OBJS) $(LIBS)

debug/%.obj : %.c
	@${CC} /c ${INCLUDE1} ${INCLUDE2} /I"../arch-win32" /I../common /Zi /nologo /W4 /WX- /Od /Oy- /D WIN32 /D _DEBUG /D _WINDOWS /D _CRT_SECURE_NO_WARNINGS /D _UNICODE /D UNICODE /Gm- /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Fo"Debug\\" /Fd"Debug\vc100.pdb" $<

clean:
	rm debug/*.*
