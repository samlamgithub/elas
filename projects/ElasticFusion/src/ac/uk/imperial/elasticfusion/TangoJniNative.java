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
    public static native String getData();
}
