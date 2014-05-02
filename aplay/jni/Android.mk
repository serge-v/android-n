LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := aplay
LOCAL_SRC_FILES := app-android.c audio.c
LOCAL_LDLIBS    := -llog -landroid -lOpenSLES
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_C_INCLUDES := .

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
