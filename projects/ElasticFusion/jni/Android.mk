LOCAL_PATH := $(call my-dir)
PROJECT_ROOT:= $(call my-dir)/../..


include $(CLEAR_VARS)
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=off
OPENCV_LIB_TYPE := STATIC
include /Users/jiahaolin/Downloads/OpenCV-android-sdk/sdk/native/jni/OpenCV.mk 

include $(CLEAR_VARS)
LOCAL_MODULE           := elasticfusion 

LOCAL_SHARED_LIBRARIES := tango_client_api \
                          ar_toolkit \
                          tango_support_api
LOCAL_CFLAGS           := -Werror -std=c++11  -O0 -g -D__ANDROID__

LOCAL_SRC_FILES := tango_handler.cc \
                   jni_interface.cc
           
LOCAL_C_INCLUDES := $(PROJECT_ROOT)/tango_gl/include \
                    $(PROJECT_ROOT)/third_party/glm/               

LOCAL_C_INCLUDES 		+= $(LOCAL_PATH)/include \
						   $(LOCAL_PATH)
						   
LOCAL_LDLIBS         += -lstdc++ -L$(SYSROOT)/usr/lib -llog -lm -lc -ldl -landroid -lGLESv3 -lz

LOCAL_CC	             := /usr/bin/gcc-4.8

include $(BUILD_SHARED_LIBRARY)
$(call import-add-path, $(LOCAL_PATH)/../..)
$(call import-module, tango_client_api)
$(call import-module, ar_toolkit)
$(call import-module, tango_support_api)
