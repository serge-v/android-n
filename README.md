android-n
=========

Attempt to develop portable apps using C and OpenGL and (Android NDK or Win32 or X11 or OSX).

Building afirst application
---------------------------

For osx:

	SRCDIR=`pwd`
	cd ~/b/afirstx
	cmake $SRCDIR/afirst

For android:

	cd ~/b/afirsta
	cmake -DCMAKE_TOOLCHAIN_FILE=~/src/xtree/android-n/afirst/android.cmake ~/src/xtree/android-n/afirst/

Install dependencies for android build:

	cd ~/b/libpng
	cmake -DCMAKE_TOOLCHAIN_FILE=$SRCDIR/afirst/android.cmake -DCMAKE_INSTALL_PREFIX=/opt/libpng ~/srcr/libpng-1.6.17
	cd ~/b/freetype
	cmake -DCMAKE_TOOLCHAIN_FILE=$SRCDIR/afirst/android.cmake -DCMAKE_INSTALL_PREFIX=/opt/freetype ~/srcr/freetype-2.5.5
