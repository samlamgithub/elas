#include <cstdlib>
#include <random>
#include <algorithm>
#include <cstdio>
#include <sstream>
#include "hello_motion_tracking/tango_handler.h"

#include "gl_util.cc"

namespace {
constexpr int kTangoCoreMinimumVersion = 9377;
void onPoseAvailable(void*, const TangoPoseData* pose) {
  LOGI("Position: %f, %f, %f. Orientation: %f, %f, %f, %f",
       pose->translation[0], pose->translation[1], pose->translation[2],
       pose->orientation[0], pose->orientation[1], pose->orientation[2],
       pose->orientation[3]);
}
}  // anonymous namespace.

namespace hello_motion_tracking {

std::unique_ptr<GlCameraFrame> TangoHandler::gl_camera_frame_;
std::unique_ptr<double> TangoHandler::frame_timestamp_;

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

  if (TangoService_connect(nullptr, tango_config_) != TANGO_SUCCESS) {
    LOGE("TangoHandler::ConnectTango, TangoService_connect error.");
    std::exit(EXIT_SUCCESS);
  }

  TangoErrorType ret = TangoConfig_setBool(tango_config_, "config_enable_color_camera", true);
   if (ret != TANGO_SUCCESS) {
     LOGE("TangoHandler Failed to enable color camera.");
     std::exit(EXIT_SUCCESS);
   }

   // Set auto-recovery for motion tracking as requested by the user.
   ret =
           TangoConfig_setBool(tango_config_, "config_enable_auto_recovery", true);
   if (ret != TANGO_SUCCESS) {
     LOGE(
             "TangoHandler: config_enable_auto_recovery() failed with error"
                     "code: %d",
             ret);
     std::exit(EXIT_SUCCESS);
   }

   // Enable depth.
   ret = TangoConfig_setBool(tango_config_, "config_enable_depth", true);
   if (ret != TANGO_SUCCESS) {
     LOGE(
             "TangoHandler: config_enable_depth() failed with error"
                     "code: %d",
             ret);
     std::exit(EXIT_SUCCESS);
   }

   // Need to specify the depth_mode as XYZC.
   ret = TangoConfig_setInt32(tango_config_, "config_depth_mode",
                              TANGO_POINTCLOUD_XYZC);
   if (ret != TANGO_SUCCESS) {
     LOGE(
             "TangoHandler: 'config_depth_mode' configuration flag with error"
                     " code: %d",
             ret);
     std::exit(EXIT_SUCCESS);
   }


  // Initialise and OpenGL camera frame class for rendering raw camera input
  gl_camera_frame_.reset(new GlCameraFrame());
  TangoCameraIntrinsics camera_intrinsics;
    TangoService_getCameraIntrinsics(camera_type_, &camera_intrinsics);
    set_frame_view_port(camera_intrinsics.width,camera_intrinsics.height);
    // Connect callbacks for new camera frames to the OpenGL GlCameraFrame class
    frame_timestamp_.reset(new double);
    TangoErrorType   status = TangoService_connectTextureId(
          camera_type_, gl_camera_frame_->texture_id(), nullptr,
          &TangoHandler::process_frame_event);
    if (status != TANGO_SUCCESS) {
      LOGE("CameraInterface: Failed to connect texture callbacks for the camera.");
      std::exit(EXIT_SUCCESS);
    }
}


void TangoHandler::set_display_view_port(int width, int height) {
  if (gl_camera_frame_) {
	  gl_camera_frame_->set_display_view_port(width,height);
  }
}

void TangoHandler::set_frame_view_port(int width, int height) {
  if (gl_camera_frame_) {
	  gl_camera_frame_->set_frame_view_port(width,height);
  }
}

int TangoHandler::get_frame_height() {
  if (gl_camera_frame_) {
    return gl_camera_frame_->get_frame_view_height();
  }
  return 0;
}

int TangoHandler::get_frame_width() {
  if (gl_camera_frame_) {
    return gl_camera_frame_->get_frame_view_width();
  }
  return 0;
}

typedef long long int timestamp_t;

// Tango specific public functions
void TangoHandler::process_frame_event(void* context, TangoCameraId id) {
	 TangoErrorType status = TangoService_updateTexture(camera_type_, frame_timestamp_.get());
	  if (status == TANGO_SUCCESS && (*frame_timestamp_) > 0) {
	    if (gl_camera_frame_) {
//	      gl_camera_frame_->render();
	      std::shared_ptr<unsigned char> frame = gl_camera_frame_->get_frame();
	      if (frame) {
	    	  timestamp_t timestamp_nanoseconds = static_cast<int64_t>((*frame_timestamp_) * 1000000000.0);
	          LOGI("raw frame(t): %lld", timestamp_nanoseconds);
	      }
	    }
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
