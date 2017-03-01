#ifndef HELLO_MOTION_TRACKING_TANGO_HANDLER_H_
#define HELLO_MOTION_TRACKING_TANGO_HANDLER_H_

#include <android/log.h>
#include <jni.h>
//#include <opencv2/core/core.hpp>

#include <cstdio>
#include <string>
#include <cstdlib>
#include <atomic>

#include "gl_util.h"
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


  // These provide the frame size for RawFrame events
   static int get_frame_height();
   static int get_frame_width();

   static void set_display_view_port(int width, int height);
     static void set_frame_view_port(int width, int height);
     static void process_frame_event(void*,TangoCameraId);
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
  const static TangoCameraId camera_type_ = TANGO_CAMERA_COLOR;
  static std::unique_ptr<GlCameraFrame> gl_camera_frame_;
  static std::unique_ptr<double> frame_timestamp_;
};
}  // namespace hello_motion_tracking

#endif  // HELLO_MOTION_TRACKING_TANGO_HANDLER_H_
