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
#define LOG_TAG "Intel HAL"
#include <cutils/log.h>
#include <utils/String8.h>
#include <AudioThrottle.h>
#include <media/AudioSystem.h>
#include <hardware/bcu_audio_interface.h>

using namespace android;

/**
 * AudioSystemSetParameters: Common interface function between Audio HAL and
 * platform specific BCU HAL to Set the Audio throttling parameter (control the
 * speaker power/volume).
 * Returns: zero (0) - setParameter success
 *          returns error code - setParameters fails
 */
static int AudioSystemSetParameters(uint8_t level)
{
    String8 param = String8(AUDIO_PARAMETER_THROTTLE_KEY) + String8("=");

    ALOGV("BCU_AUDIOIF: Inside %s Level: %d\n", __func__, level);

    switch (level) {
        case AUDIO_THROTTLE_LEVEL_0:
            param = param + AUDIO_PARAMETER_THROTTLE_LEVEL_0;
            break;
        case AUDIO_THROTTLE_LEVEL_1:
            param = param + AUDIO_PARAMETER_THROTTLE_LEVEL_1;
            break;
        case AUDIO_THROTTLE_LEVEL_2:
            param = param + AUDIO_PARAMETER_THROTTLE_LEVEL_2;
            break;
        case AUDIO_THROTTLE_LEVEL_3:
            param = param + AUDIO_PARAMETER_THROTTLE_LEVEL_3;
            break;
        default:
            ALOGW("BCU_AUDIOIF: %s unknown level %d\n", __func__, level);
            return -EINVAL;
    }
    status_t result = AudioSystem::setParameters(
                                       (audio_io_handle_t)AUDIO_IO_HANDLE_BCU,
                                       param);
    ALOGD("BCU_AUDIOIF: Audio Parameter set result = %d\n", result);

    return result;
}

/**
 * AudioSystemGetParameters: Common interface function between Audio HAL and
 * platform specific BCU HAL to get the Audio throttling parameter (current
 * speaker power consumption level).
 * Returns: throttling Level value - read success
 *          returns error code - read fails
 */
static int AudioSystemGetParameters()
{
    String8 reply;
    char *audioParam;
    int throttleLvl;

    ALOGV("BCU_AUDIOIF: Inside %s\n", __func__);
    reply = AudioSystem::getParameters(
                             (audio_io_handle_t)AUDIO_IO_HANDLE_BCU,
                             (String8)AUDIO_PARAMETER_THROTTLE_KEY);
    audioParam = strpbrk((char *)reply.string(), "=");
    ++audioParam;

    if (!strcmp(audioParam, AUDIO_PARAMETER_THROTTLE_LEVEL_0)) {
        throttleLvl = AUDIO_THROTTLE_LEVEL_0;
    } else if (!strcmp(audioParam, AUDIO_PARAMETER_THROTTLE_LEVEL_1)) {
        throttleLvl = AUDIO_THROTTLE_LEVEL_1;
    } else if (!strcmp(audioParam, AUDIO_PARAMETER_THROTTLE_LEVEL_2)) {
        throttleLvl = AUDIO_THROTTLE_LEVEL_2;
    } else if (!strcmp(audioParam, AUDIO_PARAMETER_THROTTLE_LEVEL_3)) {
        throttleLvl = AUDIO_THROTTLE_LEVEL_3;
    } else {
        ALOGW("BCU_AUDIOIF: Error in get AudioThrottle value from Audio HAL\n");
        return -ENODEV;
    }
    ALOGD("BCU_AUDIOIF: Read Audio Throttle level: %d\n", throttleLvl);

    return throttleLvl;
}

static struct audio_throttle bcu_audio_throttle = {
    .audioSetThrottle = AudioSystemSetParameters,
    .audioGetThrottle = AudioSystemGetParameters,
};

void get_bcu_audioif_info(struct audio_throttle **audioif_info)
{
    ALOGV("BCU_AUDIOIF: Inside %s\n", __func__);
    *audioif_info = &bcu_audio_throttle;
}
