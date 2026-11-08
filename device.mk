PRODUCT_SHIPPING_API_LEVEL := 21

# TWRP must accept adb without key authorization.
PRODUCT_SYSTEM_DEFAULT_PROPERTIES += \
    ro.adb.secure=0
