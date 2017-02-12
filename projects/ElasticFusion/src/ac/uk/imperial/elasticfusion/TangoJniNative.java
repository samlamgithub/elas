package ac.uk.imperial.elasticfusion;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.IBinder;
import android.util.Log;

/**
 * JNI Interface
 * @author bjm113
 *
 */

public class TangoJniNative {
    static {
//    	   if (TangoInitializationHelper.loadTangoSharedLibrary() ==
//                   TangoInitializationHelper.ARCH_ERROR) {
//               Log.e("TangoJNINative", "ERROR! Unable to load libtango_client_api.so!");
//           }
        System.loadLibrary("elasticfusion");
      }
      
//      // Camera interface functions
//      public static native boolean cameraInitialise(Activity activity,AssetManager manager);
//      public static native void cameraDestroy();
//
//      public static native boolean cameraConnect();
//      public static native void cameraDisconnect();
//
//      public static native void cameraSetDisplayViewPort(int width, int height);
//      public static native void cameraRender();
//
//      // Imu interface
//      public static native void imuOnPause();
//      public static native void imuOnResume();
//
//      // Gui interface
//      public static native boolean guiInitialise(CameraOverlay gui);
    public static native void onCreate(Activity callerActivity);
    public static native void onTangoServiceConnected(IBinder binder);
    public static native void onPause();
    
    // Allocate OpenGL resources for rendering.
    public static native void onGlSurfaceCreated();

    // Setup the view port width and height.
    public static native void onGlSurfaceChanged(int width, int height);

    // Main render loop.
    public static native void onGlSurfaceDrawFrame();

    // Set the render camera's viewing angle:
    //   first person, third person, or top down.
    public static native void setCamera(int cameraIndex);

    // Get total point count in current depth frame.
    public static native int getVerticesCount();

    // Get average depth (in meters) in current depth frame.
    public static native float getAverageZ();
    
    public static native String getData();

    // Pass touch events to the native layer.
    public static native void onTouchEvent(int touchCount, int event0,
                                           float x0, float y0, float x1, float y1);

    public static native void setScreenRotation(int orienationIndex);
}
