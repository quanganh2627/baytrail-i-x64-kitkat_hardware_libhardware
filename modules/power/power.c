/*
 * Copyright (C) 2012 The Android Open Source Project
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

#define LOG_TAG "Intel PowerHAL"
#include <utils/Log.h>

#include <hardware/hardware.h>
#include <hardware/power.h>

#define TIMER_RATE_SYSFS	"/sys/devices/system/cpu/cpufreq/interactive/timer_rate"
#define UP_THRESHOLD_SYSFS	"/sys/devices/system/cpu/cpufreq/interactive/go_hispeed_load"
#define BOOST_PULSE_SYSFS	"/sys/devices/system/cpu/cpufreq/interactive/boostpulse"

struct intel_power_module{
	struct power_module container;
	int touch_event;
	int vsync_count;
};

static void sysfs_write(char *path, char *s)
{
    char buf[80];
    int len;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);
    }

    close(fd);
}

static void intel_power_init(struct power_module *module)
{
	ALOGW("**Intel Power HAL initialisation**\n");
/*initialization*/
	sysfs_write(TIMER_RATE_SYSFS,"100000");
	sysfs_write(UP_THRESHOLD_SYSFS,"70");
}

static void intel_power_set_interactive(struct power_module *module, int on)
{
}

static void intel_power_hint(struct power_module *module, power_hint_t hint,
                       void *data) {

	struct intel_power_module *intel_module = (struct intel_power_module*)module;
	switch (hint) {
		case POWER_HINT_INTERACTION:
		if(intel_module->touch_event == 0){
			intel_module->vsync_count = 4 ;
			intel_module->touch_event = 1 ;
			sysfs_write(BOOST_PULSE_SYSFS,"1");
			sysfs_write(TIMER_RATE_SYSFS,"20000");
		}
		break;
		case POWER_HINT_VSYNC:
		if((data == 1) && (intel_module->vsync_count > 0)){
			if(intel_module->vsync_count < 4)
				sysfs_write(BOOST_PULSE_SYSFS,"1");
			intel_module->vsync_count-=1;
		}
		if((data == 0) && (intel_module->touch_event == 1)){
			sysfs_write(TIMER_RATE_SYSFS,"100000");
			sysfs_write(BOOST_PULSE_SYSFS,"1");
			intel_module->touch_event = 0;
		}
        break;
    default:
        break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};



struct intel_power_module HAL_MODULE_INFO_SYM = {
    container:{
			common: {
			tag: HARDWARE_MODULE_TAG,
			module_api_version: POWER_MODULE_API_VERSION_0_2,
			hal_api_version: HARDWARE_HAL_API_VERSION,
			id: POWER_HARDWARE_MODULE_ID,
			name: "Intel Power HAL",
			author: "Power Management Team",
			methods: &power_module_methods,
	},
    init: intel_power_init,
    setInteractive: intel_power_set_interactive,
    powerHint: intel_power_hint,
    },
	touch_event: 0,
	vsync_count: 0,
};
