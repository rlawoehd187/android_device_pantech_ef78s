LOCAL_PATH := $(call my-dir)

define ef78s-sensor-shim
include $(CLEAR_VARS)
LOCAL_MODULE := libmmcamera_$(1)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_SRC_FILES := sensor_shim.c
LOCAL_CFLAGS := -DSENSOR_NAME=\"$(1)\" -Wall -Werror
LOCAL_SHARED_LIBRARIES := libdl liblog
LOCAL_MULTILIB := 32
LOCAL_PROPRIETARY_MODULE := true
include $$(BUILD_SHARED_LIBRARY)
endef

$(eval $(call ef78s-sensor-shim,imx214))
$(eval $(call ef78s-sensor-shim,s5k5e2))
