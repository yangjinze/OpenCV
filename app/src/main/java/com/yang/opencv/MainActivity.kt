package com.yang.opencv

import android.graphics.Bitmap
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.core.graphics.drawable.toBitmap
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    val TAG = "MainActivity"
    lateinit var bitmap: Bitmap
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        bitmap = getDrawable(R.drawable.ic_launcher_background)!!.toBitmap()
        // Example of a call to a native method
        imgView.setImageBitmap(bitmap)
    }


    fun toGray(view: View) {
        Log.d(TAG, "onclick toGray")
        toGrayBitmap(bitmap)
        imgView.setImageBitmap(bitmap)
    }
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun toGrayBitmap(bitmap: Bitmap)

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
