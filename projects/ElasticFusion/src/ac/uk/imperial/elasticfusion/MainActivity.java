package ac.uk.imperial.elasticfusion;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Point;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Handler;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.TextView;
import android.util.Log;
import android.os.IBinder;

public class MainActivity extends Activity implements OnClickListener {
	private static final String TAG = MainActivity.class.getSimpleName();
	private static final int UPDATE_UI_INTERVAL_MS = 100;
	
	// Total points count in the current depth frame.
	  private TextView mPointCount;
	  // Average depth value (in meteres) of all the points in the current frame.
	  private TextView mAverageZ;

	  // GLSurfaceView and renderer, all of the graphic content is rendered
	  // through OpenGL ES 2.0 in native code.
	  private Renderer mRenderer;
	  private GLSurfaceView mGLView;
	  
	  private TextView myView;
	  private Handler mHandler = new Handler();

	  // Screen size for normalizing the touch input for orbiting the render camera.
	  private Point mScreenSize = new Point();
	  
	  // A flag to check if the Tango Service is connected. This flag avoids the
	  // program attempting to disconnect from the service while it is not
	  // connected.This is especially important in the onPause() callback for the
	  // activity class.
	  private boolean mIsConnectedService = false;
	  
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

	    // Query screen size, the screen size is used for computing the normalized
	    // touch point.
	    Display display = getWindowManager().getDefaultDisplay();
	    display.getSize(mScreenSize);
	    TangoJniNative.setScreenRotation(display.getOrientation());
		
		setContentView(R.layout.activity_main);
		
	    // Text views for the available points count.
	    mPointCount = (TextView) findViewById(R.id.point_count);

	    // Text view for average depth distance (in meters).
	    mAverageZ = (TextView) findViewById(R.id.average_depth);

	    // Buttons for selecting camera view and Set up button click listeners.
	    findViewById(R.id.first_person_button).setOnClickListener(this);
	    findViewById(R.id.third_person_button).setOnClickListener(this);
	    findViewById(R.id.top_down_button).setOnClickListener(this);
	    

	    // OpenGL view where all of the graphics are drawn.
	    mGLView = (GLSurfaceView) findViewById(R.id.gl_surface_view);

	    // Configure OpenGL renderer
	    mGLView.setEGLContextClientVersion(2);

	    // Configure the OpenGL renderer.
	    mRenderer = new Renderer();
	    mGLView.setRenderer(mRenderer);
	    myView = (TextView) findViewById(R.id.poseText);
	    
		TangoJniNative.onCreate(this);
		    
		
		    
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
	protected void onResume() {
		super.onResume();
		mGLView.onResume();
		
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

	@Override
	protected void onPause() {
		super.onPause();
		mGLView.onPause();
//		if (mIsCameraConnected) {
//			JNIInterface.cameraDisconnect();
//			JNIInterface.imuOnPause();
//		}
		  TangoJniNative.onPause();
		    mHandler.removeCallbacksAndMessages(null);
		    unbindService(mTangoServiceConnection);
	}


	  @Override
	  public void onClick(View v) {
	    // Handle button clicks.
	    switch (v.getId()) {
	    case R.id.first_person_button:
	      TangoJniNative.setCamera(0);
	      break;
	    case R.id.third_person_button:
	    	TangoJniNative.setCamera(1);
	      break;
	    case R.id.top_down_button:
	    	TangoJniNative.setCamera(2);
	      break;
	    default:
	      return;
	    }
	  }
	  

	  @Override
	  public boolean onTouchEvent(MotionEvent event) {
	    // Pass the touch event to the native layer for camera control.
	    // Single touch to rotate the camera around the device.
	    // Two fingers to zoom in and out. 
	    int pointCount = event.getPointerCount();
	    if (pointCount == 1) {
	      float normalizedX = event.getX(0) / mScreenSize.x;
	      float normalizedY = event.getY(0) / mScreenSize.y;
	      TangoJniNative.onTouchEvent(1, event.getActionMasked(),
	                                  normalizedX, normalizedY, 0.0f, 0.0f);
	    }
	    if (pointCount == 2) {
	      if (event.getActionMasked() == MotionEvent.ACTION_POINTER_UP) {
	        int index = event.getActionIndex() == 0 ? 1 : 0;
	        float normalizedX = event.getX(index) / mScreenSize.x;
	        float normalizedY = event.getY(index) / mScreenSize.y;
	        TangoJniNative.onTouchEvent(1, MotionEvent.ACTION_DOWN,
	                                    normalizedX, normalizedY, 0.0f, 0.0f);
	      } else {
	        float normalizedX0 = event.getX(0) / mScreenSize.x;
	        float normalizedY0 = event.getY(0) / mScreenSize.y;
	        float normalizedX1 = event.getX(1) / mScreenSize.x;
	        float normalizedY1 = event.getY(1) / mScreenSize.y;
	        TangoJniNative.onTouchEvent(2, event.getActionMasked(),
	                                    normalizedX0, normalizedY0,
	                                    normalizedX1, normalizedY1);
	      }
	    }
	    return true;
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
	      mPointCount.setText(String.valueOf(TangoJniNative.getVerticesCount()));
	      mAverageZ.setText(String.format("%.3f", TangoJniNative.getAverageZ()));
	    } catch (Exception e) {
	      e.printStackTrace();
	      Log.e(TAG, "Exception updateing UI elements");
	    }
	  }

}
