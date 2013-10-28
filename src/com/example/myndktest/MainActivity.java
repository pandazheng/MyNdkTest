package com.example.myndktest;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		Log.i("MainActivity-->stringFromJNI", "=====================");
		
		new Thread(){
			public void run(){
//				Log.w("MainActivity-->stringFromJNI", stringFromJNI()+"==");
//				Log.w("MainActivity-->main", main()+"==");
				setJNIEnv();
				mainThread();
			}
		}.start();
		
	}

	 //由JNI中的线程回调
    private static void fromJNI(final int i)
    {
    	Log.i("Java------>", ""+i);
//    	new Thread(){
//    		public void run(){
//    			try {
//    				while(true){
//        				Log.i("Java------>", ""+i);
//        				Thread.sleep(1500);
//        			}
//				} catch (Exception e) {
//					e.printStackTrace();
//				}
//    			
//    			
//    		}
//    	}.start();
        
    }
    
    //本地方法
    private native void mainThread();
    private native void setJNIEnv(); 
	
	//==================
	public native String stringFromJNI();
	public native int main();
	
	static {
        System.loadLibrary("MyNdkTest");
    }

}
