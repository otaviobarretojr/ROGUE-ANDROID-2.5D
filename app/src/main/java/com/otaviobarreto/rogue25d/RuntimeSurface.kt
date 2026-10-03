package com.otaviobarreto.rogue25d
import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.view.Choreographer
import android.view.View
class RuntimeSurface(context: Context): View(context), Choreographer.FrameCallback {
 private val runtime=NativeRuntime()
 private val paint=Paint(Paint.ANTI_ALIAS_FLAG)
 private var lastFrameNanos=0L
 init { setBackgroundColor(Color.rgb(18,34,28)) }
 override fun onAttachedToWindow(){ super.onAttachedToWindow(); runtime.nativeStart(); Choreographer.getInstance().postFrameCallback(this) }
 override fun onDetachedFromWindow(){ Choreographer.getInstance().removeFrameCallback(this); runtime.nativeStop(); super.onDetachedFromWindow() }
 override fun doFrame(t:Long){
  if(lastFrameNanos!=0L) runtime.nativeStep((t-lastFrameNanos)/1_000_000_000.0)
  lastFrameNanos=t; invalidate(); Choreographer.getInstance().postFrameCallback(this)
 }
 override fun onDraw(c:Canvas){
  super.onDraw(c); paint.color=Color.WHITE; paint.textSize=42f
  c.drawText("ROGUE 2.5D — Native Runtime",56f,72f,paint)
  paint.textSize=28f
  c.drawText("ARM64 bridge / frame tick: "+runtime.nativeTick(),56f,118f,paint)
  c.drawText("Prototype P1",56f,160f,paint)
 }
}
