LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := afirst
LOCAL_SRC_FILES := \
    app-android.c       \
    sound-android.c     \
    main.c              \
    font.c              \
    pumpkin.c           \
    game.c              \
    resources.c         \
    worm.c              \
    cart.c              \
    leika.c             \
    spray.c             \
    gen/bg.png.c                \
    gen/button_test.png.c       \
    gen/button_quit.png.c       \
    gen/font.png.c              \
    gen/pumpkin_red.png.c       \
    gen/pumpkin_green.png.c     \
    gen/pumpkin_yellow.png.c    \
    gen/pumpkin_mask.png.c      \
    gen/pumpkin_spoiled.png.c   \
    gen/cart.png.c              \
    gen/cart_mask.png.c         \
    gen/worm1.png.c             \
    gen/worm1_mask.png.c        \
    gen/worm2.png.c             \
    gen/worm2_mask.png.c        \
    gen/worm3.png.c             \
    gen/worm3_mask.png.c        \
    gen/leika.png.c             \
    gen/leika_mask.png.c        \
    gen/spray.png.c             \
    gen/spray_mask.png.c        \
    gen/bg.wav.c                \


LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM -lOpenSLES -lz
LOCAL_STATIC_LIBRARIES := android_native_app_glue common freetype png
LOCAL_C_INCLUDES := ../common ../freetype-2.4.10/include
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
$(call import-module,common)
$(call import-module,freetype-2.4.10)
$(call import-module,libpng)
