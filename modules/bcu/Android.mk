# Copyright (C) 2013 The Android Open Source Project 	1
# 	2
# Licensed under the Apache License, Version 2.0 (the "License"); 	3
# you may not use this file except in compliance with the License. 	4
# You may obtain a copy of the License at 	5
# 	6
#      http://www.apache.org/licenses/LICENSE-2.0 	7
# 	8
			# Unless required by applicable law or agreed to in writing, software 	9
# distributed under the License is distributed on an "AS IS" BASIS, 	10
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 	11
# See the License for the specific language governing permissions and 	12
# limitations under the License. 	13

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := bcu.default
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_C_INCLUDES := hardware/libhardware

ifeq ($(BCUHAL_MRFLD), true)
    LOCAL_SRC_FILES := \
        bcu.c \
	bcu_mrfld.c
endif

LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY
