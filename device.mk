PRODUCT_SHIPPING_API_LEVEL := 21

# TWRP must accept adb without key authorization.
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.adb.secure=0

# The 3.10 kernel has no AIO support in FunctionFS, so adbd must use blocking I/O.
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    sys.usb.ffs.aio_compat=1
