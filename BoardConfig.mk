#
# TWRP device configuration for the Pantech VEGA IM-A000S (ef78s, MSM8992).
# Kernel and dt.img are prebuilts taken from a working LineageOS 14.1 boot image.
#

DEVICE_PATH := device/pantech/ef78s

BOARD_VENDOR := pantech

ALLOW_MISSING_DEPENDENCIES := true
BUILD_BROKEN_DUP_RULES := true
BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES := true

# Architecture
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_ABI := arm64-v8a
TARGET_CPU_ABI2 :=
TARGET_CPU_VARIANT := generic

TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi
TARGET_2ND_CPU_VARIANT := generic

TARGET_BOARD_PLATFORM := msm8992
TARGET_USES_64_BIT_BINDER := true

# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := MSM8992
TARGET_NO_BOOTLOADER := true
TARGET_NO_RADIOIMAGE := true

# Kernel
BOARD_KERNEL_BASE := 0x00000000
BOARD_KERNEL_PAGESIZE := 4096
BOARD_KERNEL_IMAGE_NAME := Image
BOARD_KERNEL_CMDLINE := console=ttyHSL0,115200,n8 androidboot.console=ttyHSL0 androidboot.hardware=qcom
BOARD_KERNEL_CMDLINE += user_debug=31 msm_rtb.filter=0x37 ehci-hcd.park=3 lpm_levels.sleep_disabled=1 boot_cpus=0-5
BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive msm_poweroff.download_mode=0
TARGET_KERNEL_ARCH := arm64
TARGET_KERNEL_HEADER_ARCH := arm64
TARGET_PREBUILT_KERNEL := $(DEVICE_PATH)/prebuilt/Image
TARGET_FORCE_PREBUILT_KERNEL := true

# The bootloader reads a QCDT dt.img through the legacy dt_size header field,
# which the AOSP mkbootimg of this branch cannot produce.
BOARD_CUSTOM_MKBOOTIMG := $(DEVICE_PATH)/tools/mkbootimg_qcdt
BOARD_MKBOOTIMG_ARGS := --ramdisk_offset 0x01000000 --tags_offset 0x00000100
BOARD_MKBOOTIMG_ARGS += --dt $(DEVICE_PATH)/prebuilt/dt.img

# The kernel enables RD_GZIP, RD_BZIP2 and RD_LZMA only.
BOARD_RAMDISK_USE_LZMA := true

# Filesystem
BOARD_FLASH_BLOCK_SIZE := 262144
BOARD_BOOTIMAGE_PARTITION_SIZE := 62914560
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 62914560
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 3758096384
BOARD_HAS_LARGE_FILESYSTEM := true
TARGET_USERIMAGES_USE_EXT4 := true
TARGET_USERIMAGES_USE_F2FS := true
BOARD_SUPPRESS_SECURE_ERASE := true

# The device has no vendor partition, but TWRP installs files under /vendor in
# the recovery ramdisk, which conflicts with the default /vendor symlink.
TARGET_COPY_OUT_VENDOR := vendor

# Recovery
TARGET_RECOVERY_FSTAB := $(DEVICE_PATH)/recovery.fstab
TARGET_RECOVERY_PIXEL_FORMAT := RGBX_8888
BOARD_HAS_NO_SELECT_BUTTON := true
TARGET_USES_MKE2FS := true

# OTA
TARGET_OTA_ASSERT_DEVICE := ef78s,IM-A000S

# TWRP
TW_THEME := portrait_hdpi
RECOVERY_SDCARD_ON_DATA := true
TARGET_RECOVERY_QCOM_RTC_FIX := true
TW_BRIGHTNESS_PATH := "/sys/class/leds/lcd-backlight/brightness"
TW_MAX_BRIGHTNESS := 255
TW_DEFAULT_BRIGHTNESS := 120
TW_INCLUDE_RESETPROP := true
TW_EXTRA_LANGUAGES := false
TW_NO_USB_STORAGE := true
TW_EXCLUDE_DEFAULT_USB_INIT := true
TARGET_USE_CUSTOM_LUN_FILE_PATH := "/sys/devices/soc.0/f9200000.ssusb/f9200000.dwc3/gadget/lun%d/file"
# thermal_zone0 is the battery gauge; zone12 is tsens sensor 13 (cpu4, a big core).
TW_CUSTOM_CPU_TEMP_PATH := /sys/class/thermal/thermal_zone12/temp

# The kernel and dt.img leave about 25 MB of the 60 MiB partition for the ramdisk.
TW_USE_TOOLBOX := true
TW_EXCLUDE_APEX := true
TW_EXCLUDE_BASH := true
TW_EXCLUDE_NANO := true
TW_EXCLUDE_TWRPAPP := true
TW_EXCLUDE_TZDATA := true
