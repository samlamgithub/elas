

#ifndef HELLO_MOTION_TRACKING_TANGO_HANDLER_H_
#define HELLO_MOTION_TRACKING_TANGO_HANDLER_H_

#include <android/log.h>
#include <jni.h>

#include <cstdio>
#include <string>
#include <cstdlib>
#include <atomic>

#include "tango_client_api.h"   // NOLINT
#include "tango_support_api.h"  // NOLINT

#define LOG_TAG "cpp_hello_motion_tracking"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace hello_motion_tracking {
// TangoHandler provides functionality to communicate with the Tango Service.
class TangoHandler {
 public:
  TangoHandler() : tango_config_(nullptr) {}

  TangoHandler(const TangoHandler& other) = delete;

  TangoHandler& operator=(const TangoHandler& other) = delete;

  ~TangoHandler() {
    if (tango_config_ != nullptr) {
      TangoConfig_free(tango_config_);
      tango_config_ = nullptr;
    }
  }

  // Check if the Tango Core version is compatible with this app.
  // If not, the applicaiton will exit.
  //
  // @param env, java environment parameter CheckVersion is being called.
  // @param caller_activity, caller of this function.
  void OnCreate(JNIEnv* env, jobject caller_activity);

  // Called when the Tango service is connect. We set the binder object to Tango
  // Service in this function.
  //
  // @param env, java environment parameter.
  // @param iBinder, the native binder object.
  void OnTangoServiceConnected(JNIEnv* env, jobject iBinder);

  // Disconnect and stop Tango service.
  void OnPause();

  jstring GetData(JNIEnv* env);

 private:
  TangoConfig tango_config_;
};
}  // namespace hello_motion_tracking

#endif  // HELLO_MOTION_TRACKING_TANGO_HANDLER_H_
