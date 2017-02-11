#include <cstdlib>
#include <random>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include "hello_motion_tracking/tango_handler.h"

namespace {

constexpr int kTangoCoreMinimumVersion = 9377;

void onPoseAvailable(void*, const TangoPoseData* pose) {
  LOGI("Position: %f, %f, %f. Orientation: %f, %f, %f, %f",
       pose->translation[0], pose->translation[1], pose->translation[2],
       pose->orientation[0], pose->orientation[1], pose->orientation[2],
       pose->orientation[3]);
}

// This function logs point cloud data from OnPointCloudAvailable callbacks.
//
// @param context, this will be a pointer to a HelloDepthPerceptionApp
//        instance on which to call callbacks. This parameter is hidden
//        since it is not used.
// @param *point_cloud, point cloud data to log.
void OnPointCloudAvailable(void* /*context*/,
                           const TangoPointCloud* point_cloud) {
  // Number of points in the point cloud.
  float average_depth;

  // Calculate the average depth.
  average_depth = 0;
  // Each xyzc point has 4 coordinates.
  for (size_t i = 0; i < point_cloud->num_points; ++i) {
    average_depth += point_cloud->points[i][2];
  }
  if (point_cloud->num_points) {
    average_depth /= point_cloud->num_points;
  }

  // Log the number of points and average depth.
  LOGI("HelloDepthPerceptionApp: Point count: %d. Average depth (m): %.3f",
       point_cloud->num_points, average_depth);
}


}  // anonymous namespace.



namespace hello_motion_tracking {

void TangoHandler::OnCreate(JNIEnv* env, jobject caller_activity) {
  // Check the installed version of the TangoCore.  If it is too old, then
  // it will not support the most up to date features.
  int version = 0;
  TangoErrorType err =
      TangoSupport_GetTangoVersion(env, caller_activity, &version);
  if (err != TANGO_SUCCESS || version < kTangoCoreMinimumVersion) {
    LOGE("TangoHandler::CheckVersion, Tango Core version is out of date.");
    std::exit(EXIT_SUCCESS);
  }
}

void TangoHandler::OnTangoServiceConnected(JNIEnv* env, jobject iBinder) {
  if (TangoService_setBinder(env, iBinder) != TANGO_SUCCESS) {
    LOGE("TangoHandler::ConnectTango, TangoService_setBinder error");
    std::exit(EXIT_SUCCESS);
  }

  // TANGO_CONFIG_DEFAULT is enabling Motion Tracking and disabling Depth
  // Perception.
  tango_config_ = TangoService_getConfig(TANGO_CONFIG_DEFAULT);
  if (tango_config_ == nullptr) {
    LOGE("TangoHandler::ConnectTango, TangoService_getConfig error.");
    std::exit(EXIT_SUCCESS);
  }

  // TangoCoordinateFramePair is used to tell Tango Service about the frame of
  // references that the applicaion would like to listen to.
//  TangoCoordinateFramePair pair;
//  pair.base = TANGO_COORDINATE_FRAME_START_OF_SERVICE;
//  pair.target = TANGO_COORDINATE_FRAME_DEVICE;
//  if (TangoService_connectOnPoseAvailable(1, &pair, onPoseAvailable) !=
//      TANGO_SUCCESS) {
//    LOGE("TangoHandler::ConnectTango, connectOnPoseAvailable error.");
//    std::exit(EXIT_SUCCESS);
//  }



  // Enable Depth Perception.
  TangoErrorType err =
      TangoConfig_setBool(tango_config_, "config_enable_depth", true);
  if (err != TANGO_SUCCESS) {
    LOGE(
        "HelloDepthPerceptionApp::OnTangoServiceConnected,"
        "config_enable_depth() failed with error code: %d.",
        err);
    std::exit(EXIT_SUCCESS);
  }

  // Need to specify the depth_mode as XYZC.
  err = TangoConfig_setInt32(tango_config_, "config_depth_mode",
                             TANGO_POINTCLOUD_XYZC);
  if (err != TANGO_SUCCESS) {
    LOGE(
        "Failed to set 'depth_mode' configuration flag with error"
        " code: %d",
        err);
    std::exit(EXIT_SUCCESS);
  }

  // Attach the OnPointCloudAvailable callback to the OnPointCloudAvailable
  // function defined above. The callback will be called every time a new
  // point cloud is acquired, after the service is connected.
  err = TangoService_connectOnPointCloudAvailable(OnPointCloudAvailable);
  if (err != TANGO_SUCCESS) {
    LOGE(
        "HelloDepthPerceptionApp::OnTangoServiceConnected,"
        "Failed to connect to point cloud callback with error code: %d",
        err);
    std::exit(EXIT_SUCCESS);
  }

  if (TangoService_connect(nullptr, tango_config_) != TANGO_SUCCESS) {
    LOGE("TangoHandler::ConnectTango, TangoService_connect error.");
    std::exit(EXIT_SUCCESS);
  }
}


jstring TangoHandler::GetData(JNIEnv* env) {
    // Define what motion is requested.
    TangoCoordinateFramePair frames_of_reference;
    frames_of_reference.base = TANGO_COORDINATE_FRAME_START_OF_SERVICE;
    frames_of_reference.target = TANGO_COORDINATE_FRAME_DEVICE;
    // Query 100 poses at ~30Hz.
    TangoPoseData pose;
//        int i = 0;
//        for (i = 0; i < 100; ++i) {
//            usleep(33000);
    TangoErrorType e =   TangoService_getPoseAtTime(0.0, frames_of_reference, &pose);
    if (e == TANGO_SUCCESS) {
//            char buffer[100];
        char *hello_world = (char*)malloc(200 * sizeof(char));
//      sprintf(hello_world,"Position: %.3lf, %.3lf, %.3lf. Orientation: %.3lf, %.3lf, %.3lf, %.3lf",
//              pose.translation[0], pose.translation[1], pose.translation[2],
//              pose.orientation[0], pose.orientation[1], pose.orientation[2],
//              pose.orientation[3]);
        std::ostringstream os;
        os << "Position: ";
        os << pose.translation[0];
        os << ", ";
        os << pose.translation[1];
        os << ", ";
        os << pose.translation[2];
        os << ", ";
        os << "Orientation: ";
        os << pose.orientation[0];
        os << ", ";
        os << pose.orientation[1];
        os << ", ";
        os << pose.orientation[2];
        os << ", ";
        os << pose.orientation[3];

        std::string s(os.str());
//            jstring jstr = (env)->NewStringUTF("Hello from C");
        jstring  jstr = (env)->NewStringUTF(s.c_str());
//            free(hello_world);
        return jstr;
    } else {
        return (env)->NewStringUTF("Hello from C");
    }
        // pose now contains the latest pose data, do something interesting here.
//        }
//
}

void TangoHandler::OnPause() {
  TangoConfig_free(tango_config_);
  tango_config_ = nullptr;
  TangoService_disconnect();
}
}  // namespace hello_motion_tracking
