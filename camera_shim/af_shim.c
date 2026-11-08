/*
 * The stock AF tuning uses the layout of the Android 5.0 camera stack. Start
 * from the tuning of a libra actuator, which has every field the AF core needs,
 * and overlay the stock values that have a counterpart.
 */

#define LOG_TAG "af_shim"

#include <dlfcn.h>
#include <stdint.h>
#include <string.h>

#include <log/log.h>

#include "af_tuning_map.h"

#define LIB_DIR "/system/vendor/lib/"
#define STOCK_LIB LIB_DIR "libactuator_rumba_sa_" AF_MODE "_stock.so"
#define TEMPLATE_LIB LIB_DIR "libactuator_dw9761b_" AF_MODE ".so"
#define STOCK_SYMBOL "rumba_sa_" AF_MODE "_af_algo_open_lib"
#define TEMPLATE_SYMBOL "dw9761b_" AF_MODE "_af_algo_open_lib"

static char converted[AF_TEMPLATE_SIZE];

static const void *open_tuning(const char *path, const char *symbol)
{
    void *handle = dlopen(path, RTLD_NOW);
    void *(*open_lib)(void);

    if (!handle) {
        ALOGE("Failed to open %s: %s", path, dlerror());
        return NULL;
    }

    open_lib = (void *(*)(void))dlsym(handle, symbol);
    if (!open_lib) {
        ALOGE("Failed to find %s in %s", symbol, path);
        return NULL;
    }

    return open_lib();
}

void *AF_OPEN_SYMBOL(void)
{
    static int ready;
    const char *stock;
    const char *template_tuning;
    unsigned int i;

    if (ready)
        return converted;

    stock = open_tuning(STOCK_LIB, STOCK_SYMBOL);
    template_tuning = open_tuning(TEMPLATE_LIB, TEMPLATE_SYMBOL);
    if (!stock || !template_tuning)
        return NULL;

    memcpy(converted, template_tuning, AF_TEMPLATE_SIZE);

    /* A field the stock tuning leaves at 0 keeps the working template value */
    for (i = 0; i < sizeof(AF_MAP) / sizeof(AF_MAP[0]); i++) {
        const uint32_t *src = (const uint32_t *)(stock + AF_MAP[i].stock_offset);
        uint32_t *dst = (uint32_t *)(converted + AF_MAP[i].shim_offset);
        unsigned int word;

        for (word = 0; word < AF_MAP[i].size / sizeof(uint32_t); word++)
            if (src[word])
                dst[word] = src[word];
    }

    ALOGI("Converted the %s tuning", AF_MODE);

    ready = 1;
    return converted;
}
