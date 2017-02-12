#include <cstdlib>
#include <random>
#include <algorithm>
#include <cstdio>
#include <sstream>

#include <tango-gl/conversions.h>
#include <tango_support_api.h>
#include "hello_motion_tracking/tango_handler.h"

namespace {

constexpr int kTangoCoreMinimumVersion = 9377;

void onPoseAvailable(void* context, const TangoPoseData* pose) {
  LOGI("Position: %f, %f, %f. Orientation: %f, %f, %f, %f",
       pose->translation[0], pose->translation[1], pose->translation[2],
       pose->orientation[0], pose->orientation[1], pose->orientation[2],
       pose->orientation[3]);
}


// This function routes onPointCloudAvailable callbacks to the application
// object for handling.
//
// @param context, context will be a pointer to a PointCloudApp
//        instance on which to call callbacks.
// @param pose, pose data to route to onPoseAvailable function.
    void onPointCloudAvailableRouter(void* context,
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

    	  hello_motion_tracking::TangoHandler* handler =
              static_cast<hello_motion_tracking::TangoHandler*>(context);
    	  handler->onPointCloudAvailable(point_cloud);
    }

}  // anonymous namespace.

namespace hello_motion_tracking {

	void TangoHandler::onPointCloudAvailable(const TangoPointCloud* point_cloud) {
		 TangoSupport_updatePointCloud(point_cloud_manager_, point_cloud);
	   }

	TangoHandler::TangoHandler()
	            : screen_rotation_(0),
	              is_service_connected_(false),
	              is_gl_initialized_(false) {}

	TangoHandler::~TangoHandler() {
      if (tango_config_ != nullptr) {
        TangoConfig_free(tango_config_);
      }
      if (point_cloud_manager_ != nullptr) {
        TangoSupport_freePointCloudManager(point_cloud_manager_);
        point_cloud_manager_ = nullptr;
      }
    }

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

	  TangoSetupConfig();
	  TangoConnectCallbacks();
	  TangoConnect();
	  is_service_connected_ = true;
//	  // TANGO_CONFIG_DEFAULT is enabling Motion Tracking and disabling Depth
//	  // Perception.
//	  tango_config_ = TangoService_getConfig(TANGO_CONFIG_DEFAULT);
//	  if (tango_config_ == nullptr) {
//		LOGE("TangoHandler::ConnectTango, TangoService_getConfig error.");
//		std::exit(EXIT_SUCCESS);
//	  }

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
//
//	  // Enable Depth Perception.
//	  TangoErrorType err =
//		  TangoConfig_setBool(tango_config_, "config_enable_depth", true);
//	  if (err != TANGO_SUCCESS) {
//		LOGE(
//			"HelloDepthPerceptionApp::OnTangoServiceConnected,"
//			"config_enable_depth() failed with error code: %d.",
//			err);
//		std::exit(EXIT_SUCCESS);
//	  }
//
//	  // Need to specify the depth_mode as XYZC.
//	  err = TangoConfig_setInt32(tango_config_, "config_depth_mode",
//								 TANGO_POINTCLOUD_XYZC);
//	  if (err != TANGO_SUCCESS) {
//		LOGE(
//			"Failed to set 'depth_mode' configuration flag with error"
//			" code: %d",
//			err);
//		std::exit(EXIT_SUCCESS);
//	  }
//
//	  // Attach the OnPointCloudAvailable callback to the OnPointCloudAvailable
//	  // function defined above. The callback will be called every time a new
//	  // point cloud is acquired, after the service is connected.
//	  err = TangoService_connectOnPointCloudAvailable(OnPointCloudAvailable);
//	  if (err != TANGO_SUCCESS) {
//		LOGE(
//			"HelloDepthPerceptionApp::OnTangoServiceConnected,"
//			"Failed to connect to point cloud callback with error code: %d",
//			err);
//		std::exit(EXIT_SUCCESS);
//	  }
//
//	  if (TangoService_connect(nullptr, tango_config_) != TANGO_SUCCESS) {
//		LOGE("TangoHandler::ConnectTango, TangoService_connect error.");
//		std::exit(EXIT_SUCCESS);
//	  }
	}


    void TangoHandler::TangoSetupConfig() {
      // Here, we'll configure the service to run in the way we'd want. For this
      // application, we'll start from the default configuration
      // (TANGO_CONFIG_DEFAULT). This enables basic motion tracking capabilities.
      tango_config_ = TangoService_getConfig(TANGO_CONFIG_DEFAULT);
      if (tango_config_ == nullptr) {
        LOGE("PointCloudApp: Failed to get default config form");
        std::exit(EXIT_SUCCESS);
      }

      // Set auto-recovery for motion tracking as requested by the user.
      int ret =
              TangoConfig_setBool(tango_config_, "config_enable_auto_recovery", true);
      if (ret != TANGO_SUCCESS) {
        LOGE(
                "PointCloudApp: config_enable_auto_recovery() failed with error"
                        "code: %d",
                ret);
        std::exit(EXIT_SUCCESS);
      }

      // Enable depth.
      ret = TangoConfig_setBool(tango_config_, "config_enable_depth", true);
      if (ret != TANGO_SUCCESS) {
        LOGE(
                "PointCloudApp: config_enable_depth() failed with error"
                        "code: %d",
                ret);
        std::exit(EXIT_SUCCESS);
      }

      // Need to specify the depth_mode as XYZC.
      ret = TangoConfig_setInt32(tango_config_, "config_depth_mode",
                                 TANGO_POINTCLOUD_XYZC);
      if (ret != TANGO_SUCCESS) {
        LOGE(
                "PointCloudApp: 'config_depth_mode' configuration flag with error"
                        " code: %d",
                ret);
        std::exit(EXIT_SUCCESS);
      }

      if (point_cloud_manager_ == nullptr) {
        int32_t max_point_cloud_elements;
        ret = TangoConfig_getInt32(tango_config_, "max_point_cloud_elements",
                                   &max_point_cloud_elements);
        if (ret != TANGO_SUCCESS) {
          LOGE("Failed to query maximum number of point cloud elements.");
          std::exit(EXIT_SUCCESS);
        }

        ret = TangoSupport_createPointCloudManager(max_point_cloud_elements,
                                                   &point_cloud_manager_);
        if (ret != TANGO_SUCCESS) {
          std::exit(EXIT_SUCCESS);
        }
      }
    }

    void TangoHandler::TangoConnectCallbacks() {
      // Attach the OnPointCloudAvailable callback.
      // The callback will be called after the service is connected.
      int ret =
              TangoService_connectOnPointCloudAvailable(onPointCloudAvailableRouter);
      if (ret != TANGO_SUCCESS) {
        LOGE(
                "PointCloudApp: Failed to connect to point cloud callback with error"
                        "code: %d",
                ret);
        std::exit(EXIT_SUCCESS);
      }
    }

// Connect to the Tango Service, the service will start running:
// poses can be queried and callbacks will be called.
    void TangoHandler::TangoConnect() {
      TangoErrorType err = TangoService_connect(this, tango_config_);
      if (err != TANGO_SUCCESS) {
        LOGE(
                "PointCloudApp: Failed to connect to the Tango service with"
                        "error code: %d",
                err);
        std::exit(EXIT_SUCCESS);
      }

      // Initialize TangoSupport context.
      TangoSupport_initializeLibrary();
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
//	  TangoConfig_free(tango_config_);
//	  tango_config_ = nullptr;
//	  TangoService_disconnect();
		TangoDisconnect();
		DeleteResources();
	}


    void TangoHandler::TangoDisconnect() {
      // When disconnecting from the Tango Service, it is important to make sure to
      // free your configuration object. Note that disconnecting from the service,
      // resets all configuration, and disconnects all callbacks. If an application
      // resumes after disconnecting, it must re-register configuration and
      // callbacks with the service.
      TangoConfig_free(tango_config_);
      tango_config_ = nullptr;
      TangoService_disconnect();
      is_service_connected_ = false;
    }

    void TangoHandler::OnSurfaceCreated() { main_scene_.InitGLContent(); }

    void TangoHandler::OnSurfaceChanged(int width, int height) {
      main_scene_.SetupViewPort(width, height);
      is_gl_initialized_ = true;
    }

    void TangoHandler::OnDrawFrame() {
      main_scene_.ClearRender();

      if (!is_service_connected_ || !is_gl_initialized_) {
        return;
      }

      // Query the latest pose transformation and point cloud frame transformation.
      // Point cloud data comes in with a specific timestamp, in order to get the
      // closest pose for the point cloud, we will need to use the
      // TangoService_getMatrixTransformAtTime() to query a transform at timestamp.

      // Get the last point cloud data.
      TangoPointCloud* point_cloud = nullptr;
      TangoSupport_getLatestPointCloud(point_cloud_manager_, &point_cloud);
      if (point_cloud == nullptr) {
        return;
      }

      // Get the last device transform to start of service frame in OpenGL
      // convention.
      TangoDoubleMatrixTransformData matrix_transform;
      TangoSupport_getDoubleMatrixTransformAtTime(
              0, TANGO_COORDINATE_FRAME_START_OF_SERVICE, TANGO_COORDINATE_FRAME_DEVICE,
              TANGO_SUPPORT_ENGINE_OPENGL, TANGO_SUPPORT_ENGINE_OPENGL,
              static_cast<TangoSupportRotation>(screen_rotation_), &matrix_transform);
      if (matrix_transform.status_code == TANGO_POSE_VALID) {
        start_service_T_device_ = glm::make_mat4(matrix_transform.matrix);
      } else {
        LOGE(
                "PointCloudExample: Could not find a valid matrix transform at "
                        "time %lf for the device.",
                0.0);
        return;
      }

      // Compute the average depth value.
      float average_depth_ = 0.0f;
      for (size_t i = 0; i < point_cloud->num_points; i++) {
        average_depth_ += point_cloud->points[i][2];
      }
      if (point_cloud->num_points) {
        average_depth_ /= point_cloud->num_points;
      }
      point_cloud_average_depth_ = average_depth_;
      point_cloud_count_ = point_cloud->num_points;

      std::vector<float> vertices;
      // Get depth camera transform to start of service frame in OpenGL convention
      // at the point cloud timestamp.
      TangoSupport_getDoubleMatrixTransformAtTime(
              point_cloud->timestamp, TANGO_COORDINATE_FRAME_START_OF_SERVICE,
              TANGO_COORDINATE_FRAME_CAMERA_DEPTH, TANGO_SUPPORT_ENGINE_OPENGL,
              TANGO_SUPPORT_ENGINE_TANGO, ROTATION_IGNORED, &matrix_transform);
      if (matrix_transform.status_code == TANGO_POSE_VALID) {
        start_service_opengl_T_depth_tango_ =
                glm::make_mat4(matrix_transform.matrix);
        TangoPointCloud ow_point_cloud;
        ow_point_cloud.points = new float[point_cloud->num_points][4];
        ow_point_cloud.num_points = point_cloud->num_points;
        // Transform point cloud to OpenGL world
        TangoSupport_doubleTransformPointCloud(matrix_transform.matrix, point_cloud,
                                               &ow_point_cloud);
        vertices.resize(point_cloud->num_points * 4);
        std::copy(&ow_point_cloud.points[0][0],
                  &ow_point_cloud.points[ow_point_cloud.num_points][0],
                  vertices.begin());
        delete[] ow_point_cloud.points;
      } else {
        LOGE(
                "PointCloudExample: Could not find a valid matrix transform at "
                        "time %lf for the depth camera.",
                point_cloud->timestamp);
        return;
      }

      main_scene_.Render(start_service_T_device_, vertices);
    }

    void TangoHandler::DeleteResources() { main_scene_.DeleteResources(); }

    int TangoHandler::GetPointCloudVerticesCount() { return point_cloud_count_; }

    float TangoHandler::GetAverageZ() { return point_cloud_average_depth_; }

    void TangoHandler::SetCameraType(
            tango_gl::GestureCamera::CameraType camera_type) {
      main_scene_.SetCameraType(camera_type);
    }

    void TangoHandler::OnTouchEvent(int touch_count,
                                     tango_gl::GestureCamera::TouchEvent event,
                                     float x0, float y0, float x1, float y1) {
      if (!is_service_connected_ || !is_gl_initialized_) {
        return;
      }
      main_scene_.OnTouchEvent(touch_count, event, x0, y0, x1, y1);
    }

    void TangoHandler::SetScreenRotation(int screen_rotation) {
      screen_rotation_ = screen_rotation;
    }

}  // namespace hello_motion_tracking
