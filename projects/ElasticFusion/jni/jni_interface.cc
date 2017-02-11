
#include <jni.h>

#include "hello_motion_tracking/tango_handler.h"

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

#ifdef __cplusplus
}
#endif
