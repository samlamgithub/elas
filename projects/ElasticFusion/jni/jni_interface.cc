#define GLM_FORCE_RADIANS
#include <jni.h>

#include "hello_motion_tracking/tango_handler.h"
#include "hello_motion_tracking/scene.h"

static hello_motion_tracking::TangoHandler tango_handler;

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL

Java_ac_uk_imperial_elasticfusion_TangoJniNative_onCreate(
    JNIEnv* env, jobject /*obj*/, jobject caller_activity) {
  tango_handler.OnCreate(env, caller_activity);
}

JNIEXPORT void JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_onTangoServiceConnected(
    JNIEnv* env, jobject, jobject iBinder) {
  tango_handler.OnTangoServiceConnected(env, iBinder);
}

JNIEXPORT void JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_onPause(
    JNIEnv*, jobject) {
  tango_handler.OnPause();
}

JNIEXPORT jstring JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_getData(
        JNIEnv* env, jobject) {
  return tango_handler.GetData(env);
}


JNIEXPORT void JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_onGlSurfaceCreated(
    JNIEnv*, jobject) {
	tango_handler.OnSurfaceCreated();
}

JNIEXPORT void JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_onGlSurfaceChanged(
    JNIEnv*, jobject, jint width, jint height) {
	tango_handler.OnSurfaceChanged(width, height);
}

JNIEXPORT void JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_onGlSurfaceDrawFrame(
    JNIEnv*, jobject) {
	tango_handler.OnDrawFrame();
}

JNIEXPORT jint JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_getVerticesCount(
    JNIEnv*, jobject) {
  return tango_handler.GetPointCloudVerticesCount();
}

JNIEXPORT jfloat JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_getAverageZ(
    JNIEnv*, jobject) {
  return tango_handler.GetAverageZ();
}

JNIEXPORT void JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_setCamera(
    JNIEnv*, jobject, int camera_index) {
  tango_gl::GestureCamera::CameraType cam_type =
      static_cast<tango_gl::GestureCamera::CameraType>(camera_index);
  tango_handler.SetCameraType(cam_type);
}

JNIEXPORT void JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_onTouchEvent(
    JNIEnv*, jobject, int touch_count, int event, float x0, float y0, float x1,
    float y1) {
  tango_gl::GestureCamera::TouchEvent touch_event =
      static_cast<tango_gl::GestureCamera::TouchEvent>(event);
  tango_handler.OnTouchEvent(touch_count, touch_event, x0, y0, x1, y1);
}

JNIEXPORT void JNICALL
Java_ac_uk_imperial_elasticfusion_TangoJniNative_setScreenRotation(
    JNIEnv*, jobject, int rotation_index) {
	tango_handler.SetScreenRotation(rotation_index);
}

#ifdef __cplusplus
}
#endif
