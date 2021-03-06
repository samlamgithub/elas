#ifndef ELAS_TANGO_HANDLER_TANGO_HANDLER_H_
#define ELAS_TANGO_HANDLER_TANGO_HANDLER_H_

#include <android/log.h>
#include <jni.h>
#include <memory>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <atomic>
#include <tango-gl/util.h>

#include "../elas_tango_handler/scene.h"
#include "tango_client_api.h"   // NOLINT
#include "tango_support_api.h"  // NOLINT

#define LOG_TAG "elas_tango_demo"

namespace elas_tango_handler {
// TangoHandler provides functionality to communicate with the Tango Service.
class TangoHandler {
 public:

  TangoHandler();
  ~TangoHandler();

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


  // Tango Service point cloud callback function for depth data. Called when new
  // new point cloud data is available from the Tango Service.
  //
  // @param pose: The current point cloud returned by the service,
  //              caller allocated.
  void onPointCloudAvailable(const TangoPointCloud* point_cloud);

  // Allocate OpenGL resources for rendering, mainly for initializing the Scene.
  void OnSurfaceCreated();

  // Setup the view port width and height.
  void OnSurfaceChanged(int width, int height);

  // Main render loop.
  void OnDrawFrame();

  // Return total point count in the current depth frame.
  int GetPointCloudVerticesCount();

  // Return the average depth of points in the current depth frame.
  float GetAverageZ();
  // Set render camera's viewing angle, first person, third person or top down.
  //
  // @param: camera_type, camera type includes first person, third person and
  //         top down
  void SetCameraType(tango_gl::GestureCamera::CameraType camera_type);

  // Touch event passed from android activity. This function only supports two
  // touches.
  //
  // @param: touch_count, total count for touches.
  // @param: event, touch event of current touch.
  // @param: x0, normalized touch location for touch 0 on x axis.
  // @param: y0, normalized touch location for touch 0 on y axis.
  // @param: x1, normalized touch location for touch 1 on x axis.
  // @param: y1, normalized touch location for touch 1 on y axis.
  void OnTouchEvent(int touch_count, tango_gl::GestureCamera::TouchEvent event,
                    float x0, float y0, float x1, float y1);

  // Set screen rotation index.
  //
  // @param screen_roatation: the screen rotation index,
  //    the index is following Android screen rotation enum.
  //    see Android documentation for detail:
  //    http://developer.android.com/reference/android/view/Surface.html#ROTATION_0
  void SetScreenRotation(int rotation_index);

  jstring GetData(JNIEnv* env);

 private:

  // Setup the configuration file for the Tango Service.
  void TangoSetupConfig();

  // Connect the onPoseAvailable callback.
  void TangoConnectCallbacks();

  // Connect to Tango Service.
  // This function will start the Tango Service pipeline, in this case, it will
  // start Motion Tracking and Depth Sensing callbacks.
  void TangoConnect();

  // Disconnect from Tango Service, release all the resources that the app is
  // holding from the Tango Service.
  void TangoDisconnect();

  // Release all non-OpenGL allocated resources.
  void DeleteResources();

  // Point data manager.
  TangoSupportPointCloudManager* point_cloud_manager_;
  float point_cloud_average_depth_;
  int point_cloud_count_;

  // main_scene_ includes all drawable object for visualizing Tango device's
  // movement and point cloud.
  Scene main_scene_;

  // Tango configration file, this object is for configuring Tango Service setup
  // before connect to service. For example, we turn on the depth sensing in
  // this example.
  TangoConfig tango_config_;

  // Last valid transforms.
  glm::mat4 start_service_T_device_;
  glm::mat4 start_service_opengl_T_depth_tango_;

  // Screen rotation index.
  int screen_rotation_;

  std::atomic<bool> is_service_connected_;
  std::atomic<bool> is_gl_initialized_;
};
}  // namespace elas_tango_handler

#endif  // ELAS_TANGO_HANDLER_TANGO_HANDLER_H_
