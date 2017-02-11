LOCAL_PATH := $(call my-dir)
PROJECT_ROOT:= $(call my-dir)/../..

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

LOCAL_LDLIBS           := -landroid -llog -lGLESv3 -L$(SYSROOT)/usr/lib
LOCAL_CC	             := /usr/bin/gcc-4.8

include $(BUILD_SHARED_LIBRARY)
$(call import-add-path, $(LOCAL_PATH)/../..)
$(call import-module, tango_client_api)
$(call import-module, ar_toolkit)
$(call import-module, tango_support_api)
