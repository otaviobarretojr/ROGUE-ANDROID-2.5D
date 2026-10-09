package com.otaviobarreto.rogue25d

import android.app.Activity
import android.os.Bundle
import android.view.Gravity
import android.widget.LinearLayout
import android.widget.TextView

class MainActivity : Activity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val root = LinearLayout(this).apply {
            orientation = LinearLayout.VERTICAL
            gravity = Gravity.CENTER
        }
        root.addView(TextView(this).apply {
            text = "ROGUE 2.5D OK\nAndroid base iniciou corretamente"
            textSize = 24f
            gravity = Gravity.CENTER
        })
        setContentView(root)
    }
}
