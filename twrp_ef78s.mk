$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)
$(call inherit-product, vendor/twrp/config/common.mk)
$(call inherit-product, device/pantech/ef78s/device.mk)

PRODUCT_DEVICE := ef78s
PRODUCT_NAME := twrp_ef78s
PRODUCT_BRAND := VEGA
PRODUCT_MODEL := IM-A000S
PRODUCT_MANUFACTURER := PANTECH
