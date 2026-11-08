# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit some common Lineage stuff
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit from ef78s device
$(call inherit-product, device/pantech/ef78s/device.mk)

PRODUCT_BRAND := VEGA
PRODUCT_DEVICE := ef78s
PRODUCT_NAME := lineage_ef78s
PRODUCT_MODEL := IM-A000S
PRODUCT_MANUFACTURER := PANTECH

PRODUCT_GMS_CLIENTID_BASE := android-pantech

TARGET_VENDOR_PRODUCT_NAME := ef78s

PRODUCT_BUILD_PROP_OVERRIDES += \
    BUILD_FINGERPRINT="VEGA/VEGA_IM-A000S/ef78s:5.0.2/LRX22G/eng.dsbuild.20150403.151158:userdebug/release-keys" \
    PRIVATE_BUILD_DESC="msm8992-userdebug 5.0.2 LRX22G eng.dsbuild.20150403.151158 release-keys"
