set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_C_COMPILER   /opt/ndk-standalone-9/bin/arm-linux-androideabi-gcc)
set(CMAKE_CXX_COMPILER /opt/ndk-standalone-9/bin/arm-linux-androideabi-g++)

set(CMAKE_FIND_ROOT_PATH  /opt/ndk-standalone-9/sysroot)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(ANDROID ON)
set(ARCH android)
