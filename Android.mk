LOCAL_PATH := $(call my-dir)

ifeq ($(TARGET_DEVICE),ef78s)
include $(call all-makefiles-under,$(LOCAL_PATH))

# Remote file system layout expected by rfs_access and tftp_server
define ef78s-rfs-links
RFS_$(1)_$(2)_SYMLINKS := $(TARGET_OUT)/rfs/$(1)/$(2)/
$$(RFS_$(1)_$(2)_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "Creating RFS $(1)/$(2) links: $$@"
	@rm -rf $$@
	@mkdir -p $$@readonly
	$(hide) ln -sf /data/tombstones/$(3) $$@ramdumps
	$(hide) ln -sf /data/rfs/$(1)/$(2) $$@readwrite
	$(hide) ln -sf /data/rfs/shared $$@shared
	$(hide) ln -sf /data/hlos_rfs/shared $$@hlos
	$(hide) ln -sf /firmware $$@readonly/firmware
ALL_DEFAULT_INSTALLED_MODULES += $$(RFS_$(1)_$(2)_SYMLINKS)
endef

$(eval $(call ef78s-rfs-links,msm,adsp,lpass))
$(eval $(call ef78s-rfs-links,msm,mpss,modem))
$(eval $(call ef78s-rfs-links,mdm,adsp,lpass))
$(eval $(call ef78s-rfs-links,mdm,mpss,modem))
$(eval $(call ef78s-rfs-links,mdm,sparrow,sparrow))
$(eval $(call ef78s-rfs-links,apq,gnss,modem))
endif
