/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_HAL_INTEL_INTERFACE_H
#define ANDROID_AUDIO_HAL_INTEL_INTERFACE_H

/* Always Listening/VTSV */
#define AUDIO_PARAMETER_KEY_ALWAYS_LISTENING_STATUS "vtsv_active"
#define AUDIO_PARAMETER_VALUE_ALWAYS_LISTENING_ON "true"
#define AUDIO_PARAMETER_VALUE_ALWAYS_LISTENING_OFF "false"

/* Always Listening Route/VTSV */
#define AUDIO_PARAMETER_KEY_ALWAYS_LISTENING_ROUTE "vtsv_route"
#define AUDIO_PARAMETER_KEY_LPAL_DEVICE "lpal_device"
#define AUDIO_PARAMETER_VALUE_ALWAYS_LISTENING_ROUTE_ON "on"
#define AUDIO_PARAMETER_VALUE_ALWAYS_LISTENING_ROUTE_OFF "off"

/* Context awareness */
#define AUDIO_PARAMETER_KEY_CONTEXT_AWARENESS_STATUS "context_awareness_status"
#define AUDIO_PARAMETER_VALUE_CONTEXT_AWARENESS_ON "on"
#define AUDIO_PARAMETER_VALUE_CONTEXT_AWARENESS_OFF "off"


#endif  // ANDROID_AUDIO_INTERFACE_H