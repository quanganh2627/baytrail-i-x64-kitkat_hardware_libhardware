# Copyright (C) 2011 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := power.default
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
ifeq ($(REF_DEVICE_NAME), ctp_pr1)
	LOCAL_SRC_FILES := power_clv.c
endif
ifeq ($(REF_DEVICE_NAME), mfld_pr2)
	LOCAL_SRC_FILES := power_mfld.c
endif
ifeq ($(REF_DEVICE_NAME), mfld_gi)
	LOCAL_SRC_FILES := power_mfld.c
endif
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)
