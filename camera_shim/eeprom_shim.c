/*
 * The stock EEPROM parser fills the Android 5.0 layout of the EEPROM control
 * structure. Run it on a scratch copy and move the calibration it produced to
 * where this camera stack reads it.
 */

#define LOG_TAG "eeprom_shim"

#include <dlfcn.h>
#include <stdint.h>
#include <string.h>

#include <log/log.h>

#define STOCK_LIB "/system/vendor/lib/libmmcamera_rumba_sa_eeprom_stock.so"
#define STOCK_OPEN "rumba_sa_eeprom_open_lib"
#define STOCK_FORMAT "rumba_sa_format_calibration_data"

#define BUFFER_OFFSET 40

/* Calibration items: in-sensor, AF, white balance, lens shading, defect pixels */
#define SHIM_ITEMS_OFFSET 120

#define STOCK_WB_0 136
#define STOCK_WB_1 200
#define STOCK_WB_2 264
#define STOCK_AF_MACRO 328
#define STOCK_AF_INFINITY 330

#define SHIM_WB_0 148
#define SHIM_WB_1 212
#define SHIM_WB_2 276
#define SHIM_AF_MACRO 280
#define SHIM_AF_INFINITY 282

#define SCRATCH_SIZE 0x1000
#define FUNC_TABLE_SIZE 0x20

typedef void (*format_func)(void *);

static format_func stock_format;

static void shim_get_calibration_items(void *ctrl)
{
    uint32_t *items = (uint32_t *)((char *)ctrl + SHIM_ITEMS_OFFSET);

    items[0] = 0;
    items[1] = 1;
    items[2] = 0; /* The converted white balance data gives a black image */
    items[3] = 0;
    items[4] = 0;
}

static void shim_format_calibration_data(void *ctrl)
{
    char *shim = ctrl;
    char scratch[SCRATCH_SIZE];

    memset(scratch, 0, sizeof(scratch));
    memcpy(scratch + BUFFER_OFFSET, shim + BUFFER_OFFSET, sizeof(void *));

    stock_format(scratch);

    memcpy(shim + SHIM_WB_0, scratch + STOCK_WB_0, sizeof(uint32_t));
    memcpy(shim + SHIM_WB_1, scratch + STOCK_WB_1, sizeof(uint32_t));
    memcpy(shim + SHIM_WB_2, scratch + STOCK_WB_2, sizeof(uint32_t));
    memcpy(shim + SHIM_AF_MACRO, scratch + STOCK_AF_MACRO, sizeof(uint16_t));
    memcpy(shim + SHIM_AF_INFINITY, scratch + STOCK_AF_INFINITY, sizeof(uint16_t));

    ALOGI("AF macro %u infinity %u",
            *(uint16_t *)(shim + SHIM_AF_MACRO),
            *(uint16_t *)(shim + SHIM_AF_INFINITY));
}

void *rumba_sa_eeprom_open_lib(void)
{
    static void *table[FUNC_TABLE_SIZE / sizeof(void *)];
    static int ready;
    void *handle;
    void *(*stock_open)(void);
    void *stock_table;

    if (ready)
        return table;

    handle = dlopen(STOCK_LIB, RTLD_NOW);
    if (!handle) {
        ALOGE("Failed to open %s: %s", STOCK_LIB, dlerror());
        return NULL;
    }

    stock_open = (void *(*)(void))dlsym(handle, STOCK_OPEN);
    stock_format = (format_func)dlsym(handle, STOCK_FORMAT);
    if (!stock_open || !stock_format) {
        ALOGE("Missing symbols in %s", STOCK_LIB);
        return NULL;
    }

    stock_table = stock_open();
    if (!stock_table)
        return NULL;

    memcpy(table, stock_table, FUNC_TABLE_SIZE);
    table[0] = (void *)shim_get_calibration_items;
    table[1] = (void *)shim_format_calibration_data;

    ready = 1;
    return table;
}
