PRODUCT_RELEASE_NAME := ef78s

$(call inherit-product, build/target/product/embedded.mk)
$(call inherit-product, vendor/omni/config/common.mk)
$(call inherit-product, device/pantech/ef78s/device.mk)

PRODUCT_DEVICE := ef78s
PRODUCT_NAME := omni_ef78s
PRODUCT_BRAND := VEGA
PRODUCT_MODEL := IM-A000S
PRODUCT_MANUFACTURER := PANTECH
