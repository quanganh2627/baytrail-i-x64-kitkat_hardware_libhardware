/*
 * Copyright (C) 2013 The Android Open Source Project
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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#define LOG_TAG "Intel MRFL BCUHAL"
#include <utils/Log.h>

#include <hardware/bcu_audio_interface.h>
#include <hardware/hardware.h>
#include <hardware/bcu.h>

#define BCU_CAMFLASH_SYSFS \
            "/sys/devices/platform/bcove_bcu/camflash_ctrl"

#define CAMFLASH_MIN_THROTTLE_LVL         0
#define CAMFLASH_MAX_THROTTLE_LVL         3
#define MAXSYSFS_DATA_SIZ                 8

struct audio_throttle *bcu_audioif;

/**
 * Generic function to write the data into sysfs interface (path which is
 * provided as a input argument in path).
 * Returns: zero (0) - write success
 *          returns error code - write fails
 */
static int write_sysfs(char const *path, uint8_t data)
{
    char buf[80];
    int fd = open(path, O_WRONLY);
    if (fd >= 0) {
        char buffer[MAXSYSFS_DATA_SIZ] = {'\0'};
        int bytes = snprintf(buffer, sizeof(buffer), "%d\n", data);
        int amt = write(fd, buffer, bytes);
        if (amt < 0) {
            strerror_r(errno, buf, sizeof(buf));
            ALOGE("MRFL BCU_HAL: Error writing into %s: %s\n", path, buf);
        }
        close(fd);
        return amt == -1 ? -errno : 0;
    } else {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("MRFL BCU_HAL: Error opening %s: %s\n", path, buf);
        return -errno;
    }
}

/**
 * Generic function to read the sysfs interface (path which is provided as a
 * input argument in path) and returns with the read value as a string in *s.
 * Returns: number of bytes read - read success
 *          returns error code - read fails
 */
static int read_sysfs(char const *path, char *data, int num_bytes)
{
    char buf[80];
    int count;
    int fd = open(path, O_RDONLY);
    if (fd >= 0) {
        char buffer[20] = {'\0'};
        int amt = read(fd, data, num_bytes);
        if (amt < 0) {
            strerror_r(errno, buf, sizeof(buf));
            ALOGE("MRFL BCU_HAL: Error reading from %s: %s\n", path, buf);
        }
        close(fd);
        return amt == -1 ? -errno : amt;
    } else {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("MRFL BCU_HAL: Error opening %s: %s\n", path, buf);
        return -errno;
    }
}

int mrfl_setaudio_throttle(uint8_t level)
{
    ALOGD("MRFL BCU_HAL: %s level %d\n", __func__, level);
    /* Audio HAL interface function to throttle the speaker power */
    return bcu_audioif->audioSetThrottle(level);
}

int mrfl_setcamera_throttle(uint8_t level)
{
    ALOGD("MRFL BCU_HAL: %s level %d\n", __func__, level);

    if ((level >= CAMFLASH_MIN_THROTTLE_LVL)
           && (level <= CAMFLASH_MAX_THROTTLE_LVL))
        return write_sysfs(BCU_CAMFLASH_SYSFS, level);
    else
        return -EINVAL;
}

int mrfl_setdisplay_throttle(uint8_t level)
{
    ALOGD("MRFL BCU_HAL: %s level %d\n", __func__, level);
    /* TODO: Need to add the respective HAL Function Call */
    return 0;
}

int mrfl_setotg_throttle(uint8_t level)
{
    ALOGD("MRFL BCU_HAL: %s level %d\n", __func__, level);
    /* TODO: Need to add the respective HAL Function Call */
    return 0;
}

int mrfl_setvibra_throttle(uint8_t level)
{
    ALOGD("MRFL BCU_HAL: %s level %d\n", __func__, level);
    /* TODO: Need to add the respective HAL Function Call */
    return 0;
}

int mrfl_getaudio_throttle(uint8_t *level)
{
    int ret = 0;

    /* Audio HAL interface function to get the current audio throttle level */
    ret = bcu_audioif->audioGetThrottle();
    if (ret < 0) {
        ALOGE("MRFL BCU_HAL: Error in %s error code: %d\n", __func__,ret);
        return ret;
    }

    *level = ret;
    return 0;
}

int mrfl_getcamera_throttle(uint8_t *level)
{
    int ret;
    char buf[MAXSYSFS_DATA_SIZ] = {'\0'};

    ret = read_sysfs(BCU_CAMFLASH_SYSFS, buf, MAXSYSFS_DATA_SIZ);
    if (ret < 0) {
        ALOGE("MRFL BCU_HAL: Error in %s\n", __func__);
        return ret;
    } else {
        ret = 0;
        uint8_t data = atoi(buf);
        if ((data >= CAMFLASH_MIN_THROTTLE_LVL)
               && (data <= CAMFLASH_MAX_THROTTLE_LVL))
            *level = data;
        else
            ret = -ENODATA;
    }
    return ret;
}

int mrfl_getdisplay_throttle(uint8_t *level)
{
    int ret = 0;

    /* ret = TODO: Need to add the respective HAL Function Call */
    if (ret < 0) {
        ALOGE("MRFL BCU_HAL: Error in %s\n", __func__);
        return ret;
    }

    *level = ret;
    return 0;
}

int mrfl_getotg_throttle(uint8_t *level)
{
    int ret = 0;

    /* ret = TODO: Need to add the respective HAL Function Call */
    if (ret < 0) {
        ALOGE("MRFL BCU_HAL: Error in %s\n", __func__);
        return ret;
    }

    *level = ret;
    return 0;
}

int mrfl_getvibra_throttle(uint8_t *level)
{
    int ret = 0;

    /* ret = TODO: Need to add the respective HAL Function Call */
    if (ret < 0) {
        ALOGE("MRFL BCU_HAL: Error in %s\n", __func__);
        return ret;
    }

    *level = ret;
    return 0;
}

static struct bcu_throttle mrfl_bcu_throttle = {
    .setAudioThrottle = mrfl_setaudio_throttle,
    .setCameraThrottle = mrfl_setcamera_throttle,
    .setDisplayThrottle = mrfl_setdisplay_throttle,
    .setOtgThrottle = mrfl_setotg_throttle,
    .setVibraThrottle = mrfl_setvibra_throttle,
    .getAudioThrottle = mrfl_getaudio_throttle,
    .getCameraThrottle = mrfl_getcamera_throttle,
    .getDisplayThrottle = mrfl_getdisplay_throttle,
    .getOtgThrottle = mrfl_getotg_throttle,
    .getVibraThrottle = mrfl_getvibra_throttle,
};

void get_throttle_info(struct bcu_throttle **throttle_info)
{
    ALOGI("MRFL BCU_HAL: Inside %s\n", __func__);

    /* getting audio interface throttling info for get/set audio throttling */
    get_bcu_audioif_info(&bcu_audioif);

    /**
    * providing the initialized platform specific throttling info to get/set
    * the bcu throttling.
    */
    *throttle_info = &mrfl_bcu_throttle;
}
