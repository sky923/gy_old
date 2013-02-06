# Copyright (C) 2009 The Android Open Source Project
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
LOCAL_PATH := $(call my-dir)/../..

include $(CLEAR_VARS)
LOCAL_MODULE    := libgy
LOCAL_SRC_FILES := \
	../pil/gyInputAndroid.cpp \
	../pil/gyRenderer.cpp \
	../pil/gyRendererAndroid.cpp \
	../pil/gyViewAndroid.cpp \
	../pil/gyViewAndroidNativeActivity.cpp \
	../pil/gyMainCore.cpp \
	../gyMain.cpp

LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2 -lOpenSLES
LOCAL_CFLAGS := -Wno-psabi -std=gnu++11 -D__ANDROID__ -I"../.."
# -I"../../external-deps/lua/include" -I"../../external-deps/bullet/include" -I"../../external-deps/libpng/include" -I"../../external-deps/oggvorbis/include" -I"../../external-deps/openal/include"
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_STATIC_LIBRARY)

$(call import-module,android/native_app_glue)
