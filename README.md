android-n
=========

Cross platform apps for Android and desktop (Win32, X11, OSX) using C and OpenGL.

Building afirst application
---------------------------

For osx:

	SRCDIR=`pwd`
	cd ~/b/afirstx
	cmake $SRCDIR/afirst

For android:

	cd ~/b/afirsta
	cmake -DCMAKE_TOOLCHAIN_FILE=$SRCDIR/afirst/android.cmake ~/src/xtree/android-n/afirst/

Install dependencies for android build:

	cd ~/b/libpng
	
	cmake \
		-DCMAKE_TOOLCHAIN_FILE=$SRCDIR/afirst/android.cmake \
		-DCMAKE_INSTALL_PREFIX=/opt/libpng \
		~/srcr/libpng-1.6.17
	
	cd ~/b/freetype

	cmake \
		-DCMAKE_TOOLCHAIN_FILE=$SRCDIR/afirst/android.cmake \
		-DCMAKE_INSTALL_PREFIX=/opt/freetype \
		~/srcr/freetype-2.5.5

Install to android:

	/opt/android-sdk/platform-tools/adb install -r android/bin/afirst-debug.apk

View log:

	/opt/android-sdk/platform-tools/adb logcat

View smbols:

	/opt/ndk-standalone-9/bin/arm-linux-androideabi-objdump -T android/libs/armeabi/libafirst.so
