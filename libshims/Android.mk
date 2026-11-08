LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libshim_adreno
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := adreno.c
LOCAL_C_INCLUDES := external/boringssl/include
LOCAL_SHARED_LIBRARIES := libcrypto
include $(BUILD_SHARED_LIBRARY)
