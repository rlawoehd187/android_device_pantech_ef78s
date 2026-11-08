/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "lights.ef78s"

#include <cutils/log.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hardware/lights.h>

#define LCD_FILE   "/sys/class/leds/lcd-backlight/brightness"
#define RED_FILE   "/sys/class/leds/led:rgb_red/brightness"
#define GREEN_FILE "/sys/class/leds/led:rgb_green/brightness"
#define BLUE_FILE  "/sys/class/leds/led:rgb_blue/brightness"

static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static struct light_state_t g_notification;
static struct light_state_t g_battery;
static struct light_state_t g_attention;

static int write_int(const char *path, int value)
{
    char buf[16];
    int fd = open(path, O_WRONLY);
    int len, ret = 0;

    if (fd < 0) {
        ALOGE("failed to open %s: %s", path, strerror(errno));
        return -errno;
    }
    len = snprintf(buf, sizeof(buf), "%d\n", value);
    if (write(fd, buf, len) != len)
        ret = -errno;
    close(fd);
    return ret;
}

static int is_lit(const struct light_state_t *state)
{
    return state->color & 0x00ffffff;
}

static int rgb_to_brightness(const struct light_state_t *state)
{
    int c = state->color & 0x00ffffff;
    return ((77 * ((c >> 16) & 0xff)) + (150 * ((c >> 8) & 0xff)) +
            (29 * (c & 0xff))) >> 8;
}

static int set_light_backlight(struct light_device_t *dev,
                               const struct light_state_t *state)
{
    int err;

    (void)dev;
    pthread_mutex_lock(&g_lock);
    err = write_int(LCD_FILE, rgb_to_brightness(state));
    pthread_mutex_unlock(&g_lock);
    return err;
}

/* The qpnp RGB LEDs have no blink node, so a blinking request is shown solid. */
static void update_indicator(void)
{
    const struct light_state_t *state = &g_battery;
    unsigned int c;

    if (is_lit(&g_attention))
        state = &g_attention;
    else if (is_lit(&g_notification))
        state = &g_notification;

    c = state->color;
    write_int(RED_FILE, (c >> 16) & 0xff);
    write_int(GREEN_FILE, (c >> 8) & 0xff);
    write_int(BLUE_FILE, c & 0xff);
}

#define DEFINE_SETTER(name, slot)                                   \
static int name(struct light_device_t *dev,                         \
                const struct light_state_t *state)                  \
{                                                                   \
    (void)dev;                                                      \
    pthread_mutex_lock(&g_lock);                                    \
    slot = *state;                                                  \
    update_indicator();                                             \
    pthread_mutex_unlock(&g_lock);                                  \
    return 0;                                                       \
}

DEFINE_SETTER(set_light_notifications, g_notification)
DEFINE_SETTER(set_light_battery, g_battery)
DEFINE_SETTER(set_light_attention, g_attention)

static int close_lights(struct light_device_t *dev)
{
    free(dev);
    return 0;
}

static int open_lights(const struct hw_module_t *module, const char *name,
                       struct hw_device_t **device)
{
    int (*set_light)(struct light_device_t *, const struct light_state_t *);
    struct light_device_t *dev;

    if (!strcmp(LIGHT_ID_BACKLIGHT, name))
        set_light = set_light_backlight;
    else if (!strcmp(LIGHT_ID_NOTIFICATIONS, name))
        set_light = set_light_notifications;
    else if (!strcmp(LIGHT_ID_BATTERY, name))
        set_light = set_light_battery;
    else if (!strcmp(LIGHT_ID_ATTENTION, name))
        set_light = set_light_attention;
    else
        return -EINVAL;

    dev = calloc(1, sizeof(*dev));
    if (!dev)
        return -ENOMEM;

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t *)module;
    dev->common.close = (int (*)(struct hw_device_t *))close_lights;
    dev->set_light = set_light;

    *device = (struct hw_device_t *)dev;
    return 0;
}

static struct hw_module_methods_t lights_module_methods = {
    .open = open_lights,
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = LIGHTS_HARDWARE_MODULE_ID,
    .name = "ef78s lights module",
    .author = "The LineageOS Project",
    .methods = &lights_module_methods,
};
