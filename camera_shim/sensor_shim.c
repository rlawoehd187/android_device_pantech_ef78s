/*
 * The stock sensor libraries lay out sensor_lib_t for the Android 5.0 camera
 * stack. The sensor module in use expects a shorter chromatix table, so give
 * it a converted copy of the stock structure.
 */

#define LOG_TAG "sensor_shim"

#include <dlfcn.h>
#include <stdint.h>
#include <string.h>

#include <log/log.h>

#define STOCK_LIB "/system/vendor/lib/libmmcamera_" SENSOR_NAME "_stock.so"

#define CHROMATIX_OFFSET 0x1424
#define CHROMATIX_ENTRIES 10
#define STOCK_ENTRY_WORDS 12
#define SHIM_ENTRY_WORDS 11
#define STOCK_TAIL_OFFSET 0x1604
#define SHIM_TAIL_OFFSET 0x15dc
#define TAIL_SIZE 0x9c
#define TAIL_HEAD_SIZE 0xc
#define TAIL_SKIP_END 0x74
#define EEPROM_NAME_OFFSET 0x20
#define EEPROM_NAME_SIZE 0x20
#define ACTUATOR_NAME_OFFSET 0x40
#define ACTUATOR_NAME_SIZE 0x20
#define SHIM_STRUCT_SIZE 0x3000

static char converted[SHIM_STRUCT_SIZE];

void *sensor_open_lib(void)
{
    static int ready;
    void *handle;
    void *(*stock_open)(void);
    const char *stock;
    int i;

    if (ready)
        return converted;

    handle = dlopen(STOCK_LIB, RTLD_NOW);
    if (!handle) {
        ALOGE("Failed to open %s: %s", STOCK_LIB, dlerror());
        return NULL;
    }

    stock_open = (void *(*)(void))dlsym(handle, "sensor_open_lib");
    if (!stock_open) {
        ALOGE("Failed to find sensor_open_lib in %s", STOCK_LIB);
        return NULL;
    }

    stock = stock_open();
    if (!stock) {
        ALOGE("%s returned no sensor library", STOCK_LIB);
        return NULL;
    }

    memcpy(converted, stock, CHROMATIX_OFFSET);

    /* The stock EEPROM library has an incompatible function table, so skip it */
    memset(converted + EEPROM_NAME_OFFSET, 0, EEPROM_NAME_SIZE);

    /* The stock AF tuning does not match this AF core, so run with fixed focus */
    memset(converted + ACTUATOR_NAME_OFFSET, 0, ACTUATOR_NAME_SIZE);

    for (i = 0; i < CHROMATIX_ENTRIES; i++) {
        const uint32_t *src = (const uint32_t *)(stock + CHROMATIX_OFFSET) +
                i * STOCK_ENTRY_WORDS;
        uint32_t *dst = (uint32_t *)(converted + CHROMATIX_OFFSET) +
                i * SHIM_ENTRY_WORDS;

        /* common, preview, preview, video, liveshot, cpp preview, cpp snapshot */
        memcpy(dst, src, 7 * sizeof(uint32_t));
        /* The extra snapshot slot has no stock counterpart */
        dst[7] = src[6];
        /* cpp video, cpp liveshot, postproc */
        memcpy(dst + 8, src + 7, 3 * sizeof(uint32_t));
    }

    /* Chromatix count and the two pointers after it */
    memcpy(converted + SHIM_TAIL_OFFSET, stock + STOCK_TAIL_OFFSET, TAIL_HEAD_SIZE);
    /* The fields between them are PDAF settings the stock libraries do not have */
    memcpy(converted + SHIM_TAIL_OFFSET + TAIL_SKIP_END,
            stock + STOCK_TAIL_OFFSET + TAIL_SKIP_END,
            TAIL_SIZE - TAIL_SKIP_END);

    ready = 1;
    return converted;
}
