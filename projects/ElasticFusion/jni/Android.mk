LOCAL_PATH := $(call my-dir)
PROJECT_ROOT_FROM_JNI:= ../..
PROJECT_ROOT:= $(call my-dir)/../..

include $(CLEAR_VARS)
LOCAL_MODULE           := elasticfusion 

LOCAL_SHARED_LIBRARIES := tango_client_api \
                          ar_toolkit \
                          tango_3d_reconstruction \
                          tango_support_api
LOCAL_STATIC_LIBRARIES := png
LOCAL_CFLAGS           := -Werror -std=c++11  -O0 -g -D__ANDROID__

LOCAL_SRC_FILES := point_cloud_drawable.cc \
                   scene.cc \
				   tango_handler.cc \
                   jni_interface.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/axis.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/camera.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/conversions.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/drawable_object.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/frustum.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/gesture_camera.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/grid.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/tango_gl.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/line.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/shaders.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/trace.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/texture.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/transform.cc \
                   $(PROJECT_ROOT_FROM_JNI)/tango_gl/util.cc
           
LOCAL_C_INCLUDES := $(PROJECT_ROOT)/tango-service-sdk/include/ \
					$(PROJECT_ROOT)/tango_gl/include \
                    $(PROJECT_ROOT)/third_party/glm/               

LOCAL_LDLIBS           := -landroid -llog -lGLESv3 -L$(SYSROOT)/usr/lib
LOCAL_CC	             := /usr/bin/gcc-4.8

include $(BUILD_SHARED_LIBRARY)
$(call import-add-path, $(LOCAL_PATH)/../..)
$(call import-module, tango_client_api)
$(call import-module,libpng)
$(call import-module, ar_toolkit)
$(call import-module,tango_3d_reconstruction)
$(call import-module, tango_support_api)
