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

define ef78s-af-shim
include $(CLEAR_VARS)
LOCAL_MODULE := libactuator_rumba_sa_$(1)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_SRC_FILES := af_shim.c
LOCAL_CFLAGS := -DAF_MODE=\"$(1)\" -DAF_OPEN_SYMBOL=rumba_sa_$(1)_af_algo_open_lib -DAF_MAP=af_$(1)_map -Wall -Werror -Wno-unused-variable
LOCAL_SHARED_LIBRARIES := libdl liblog
LOCAL_MULTILIB := 32
LOCAL_PROPRIETARY_MODULE := true
include $$(BUILD_SHARED_LIBRARY)
endef

$(eval $(call ef78s-af-shim,camera))
$(eval $(call ef78s-af-shim,camcorder))
