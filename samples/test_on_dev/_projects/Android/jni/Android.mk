# Copyright (C) 2010 The Android Open Source Project
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
#

GY_PROJECT_PATH := $(call my-dir)/../..

# libgy
LOCAL_PATH := $(call my-dir)/../../../gy/android/obj/local/armeabi
include $(CLEAR_VARS)
LOCAL_MODULE    := libgy
LOCAL_SRC_FILES := libgy.a
include $(PREBUILT_STATIC_LIBRARY)

# test01
LOCAL_PATH := $(GY_PROJECT_PATH)
include $(CLEAR_VARS)
LOCAL_MODULE    := test01
LOCAL_SRC_FILES := main.cpp
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES
LOCAL_CFLAGS    := -Wno-psabi -std=gnu++0x -D__ANDROID__ -I"../../gy"
LOCAL_STATIC_LIBRARIES := android_native_app_glue libgy

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
