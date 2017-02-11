package ac.uk.imperial.elasticfusion;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.util.Log;
import android.os.IBinder;

public class MainActivity extends Activity {
	private static final String TAG = MainActivity.class.getSimpleName();
	  private TextView myView;
	  private Handler mHandler = new Handler();
//	private FrameLayout mFrameLayout;

//	private boolean mIsCameraConnected = false;
//	private GLSurfaceRenderer mRenderer;
//	private GLSurfaceView mGLView;

//	private CameraOverlay mOverlay;//action_settings
	
	 ServiceConnection mTangoServiceConnection = new ServiceConnection() {
	      public void onServiceConnected(ComponentName name, IBinder service) {
	        TangoJniNative.onTangoServiceConnected(service);
	      }

	      public void onServiceDisconnected(ComponentName name) {
	        // Handle this if you need to gracefully shutdown/retry
	        // in the event that Tango itself crashes/gets upgraded while running.
	      }
	    };


	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		 setContentView(R.layout.activity_main);
		    TangoJniNative.onCreate(this);
		    myView = (TextView) findViewById(R.id.poseText);
//		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
//				WindowManager.LayoutParams.FLAG_FULLSCREEN);
//		JNIInterface.cameraInitialise(this, getAssets());
//		mFrameLayout = new FrameLayout(this);
//		mOverlay = new CameraOverlay(this);
//		addContentView(mOverlay, new LayoutParams(LayoutParams.MATCH_PARENT,
//				LayoutParams.MATCH_PARENT));
//		mGLView = new GLSurfaceView(this);
//		mRenderer = new GLSurfaceRenderer(this, mOverlay);
//		mGLView.setRenderer(mRenderer);
//		mGLView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
//		addContentView(mGLView, new LayoutParams(LayoutParams.MATCH_PARENT,
//				LayoutParams.MATCH_PARENT));
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		super.onPause();
//		mGLView.onPause();
//		if (mIsCameraConnected) {
//			JNIInterface.cameraDisconnect();
//			JNIInterface.imuOnPause();
//		}
		  TangoJniNative.onPause();
		    mHandler.removeCallbacksAndMessages(null);
		    unbindService(mTangoServiceConnection);
	}

	@Override
	protected void onResume() {
		super.onResume();
		Intent intent = new Intent();
		intent.setClassName("com.google.tango", "com.google.atap.tango.TangoService");
		if (!bindService(intent, mTangoServiceConnection, Context.BIND_AUTO_CREATE)) {
			Log.e(TAG, "error binding");
		}
//		mGLView.onResume();
//		JNIInterface.imuOnResume();
		  // Start the debug text UI update loop.
	    mHandler.postDelayed(mUpdateUiLoopRunnable, 5000);
	}

//	public void surfaceCreated() {
//		mIsCameraConnected = JNIInterface.cameraConnect();
//		if (!mIsCameraConnected) {
//			Log.e(TAG, "Failed to connect to camera");
//			finish();
//		}
//	}
//
//	public void requestRender() {
//		mGLView.requestRender();
//	}
	private static final int UPDATE_UI_INTERVAL_MS = 100;
	  // Debug text UI update loop, updating at 10Hz.
	  private Runnable mUpdateUiLoopRunnable = new Runnable() {
	    public void run() {

	      updateUi();
	      mHandler.postDelayed(this, UPDATE_UI_INTERVAL_MS);
	    }
	  };

	  // Update the debug text UI.
	  private void updateUi() {
	    try {
	      myView.setText(TangoJniNative.getData());
	    } catch (Exception e) {
	      e.printStackTrace();
	      Log.e(TAG, "Exception updateing UI elements");
	    }
	  }

}
