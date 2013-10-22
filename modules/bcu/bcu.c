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
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<time.h>

#define LOG_TAG "Intel BCUHAL"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/bcu.h>

struct bcu_throttle *throttle;

struct intel_bcu_module {
    struct bcu_module container;
};

static void intel_bcu_init(struct bcu_module *module)
{
    ALOGW("**Intel BCU HAL initialisation**\n");
    get_throttle_info(&throttle);
}

static int intel_setthrottale_level(enum sub_system subsys,
               uint8_t level) {
    int ret;

    ALOGD("BCU_HAL: Set Throttale Level %d subsys %d\n", level, subsys);

    switch (subsys) {
        case BCU_SUBSYS_AUDIO:
            ret = throttle->setAudioThrottle(level);
            break;
        case BCU_SUBSYS_CAMERA:
            ret = throttle->setCameraThrottle(level);
            break;
        case BCU_SUBSYS_DISPLAY:
            ret = throttle->setDisplayThrottle(level);
            break;
        case BCU_SUBSYS_OTG:
            ret = throttle->setOtgThrottle(level);
            break;
        case BCU_SUBSYS_VIBRA:
            ret = throttle->setVibraThrottle(level);
            break;
        default:
            ALOGE("BCU_HAL: Inside %s default case\n", __func__);
            ret = -ENODEV;
    }

    return ret;
}

static int intel_getthrottale_level(enum sub_system subsys,
               uint8_t *level) {
    int ret;

    ALOGD("BCU_HAL: Get Throttale Level for subsys %d\n", subsys);

    switch (subsys) {
        case BCU_SUBSYS_AUDIO:
            ret = throttle->getAudioThrottle(level);
             break;
        case BCU_SUBSYS_CAMERA:
             ret = throttle->getCameraThrottle(level);
             break;
        case BCU_SUBSYS_DISPLAY:
             ret = throttle->getDisplayThrottle(level);
             break;
        case BCU_SUBSYS_OTG:
             ret = throttle->getOtgThrottle(level);
             break;
        case BCU_SUBSYS_VIBRA:
             ret = throttle->getVibraThrottle(level);
             break;
        default:
             ALOGE("BCU_HAL: Inside %s default case\n", __func__);
             ret = -ENODEV;
    }

    return ret;
}

static struct hw_module_methods_t bcu_module_methods = {
    .open = NULL,
};

struct intel_bcu_module HAL_MODULE_INFO_SYM = {
    container: {
        common: {
            tag: HARDWARE_MODULE_TAG,
            module_api_version : BCU_MODULE_API_VERSION_0_2,
            hal_api_version : HARDWARE_HAL_API_VERSION,
            id : BCU_HARDWARE_MODULE_ID,
            name : "Intel BCU HAL",
            author : "Energy Management Team",
            methods : &bcu_module_methods,
        },
        init : intel_bcu_init,
        setThrottleLevel : intel_setthrottale_level,
        getThrottleLevel : intel_getthrottale_level,
    },
};
