package com.otaviobarreto.rogue25d
import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.view.Choreographer
import android.view.MotionEvent
import android.view.View

class RuntimeSurface(context: Context): View(context), Choreographer.FrameCallback {
 private var runtime:NativeRuntime?=null
 private val paint=Paint(Paint.ANTI_ALIAS_FLAG)
 private var lastFrameNanos=0L
 private var buttons=0
 private var nativeReady=false
 private var startupError:String?=null
 init {
  setBackgroundColor(Color.rgb(18,34,28))
 }
 override fun onAttachedToWindow(){
  super.onAttachedToWindow()
  lastFrameNanos=0L
  try {
   NativeRuntime.load().getOrThrow()
   val activeRuntime=NativeRuntime()
   runtime=activeRuntime
   activeRuntime.nativeSetStoragePath(context.filesDir.absolutePath)
   activeRuntime.nativeStart()
   nativeReady=true
   startupError=null
   Choreographer.getInstance().postFrameCallback(this)
  } catch(t:Throwable) {
   nativeReady=false
   startupError=t.javaClass.simpleName+": "+(t.message ?: "native startup failed")
   invalidate()
  }
 }
 override fun onDetachedFromWindow(){
  Choreographer.getInstance().removeFrameCallback(this)
  lastFrameNanos=0L
  buttons=0
  if(nativeReady) {
   runtime?.nativeSetButtons(0)
   runtime?.nativeStop()
  }
  nativeReady=false
  super.onDetachedFromWindow()
 }
 override fun doFrame(t:Long){
  if(!nativeReady) return
  if(lastFrameNanos!=0L) runtime?.nativeStep((t-lastFrameNanos)/1_000_000_000.0)
  lastFrameNanos=t; invalidate(); Choreographer.getInstance().postFrameCallback(this)
 }
 override fun onTouchEvent(e:MotionEvent):Boolean {
  if(e.actionMasked==MotionEvent.ACTION_UP || e.actionMasked==MotionEvent.ACTION_CANCEL){
   buttons=0
  } else {
   val x=e.x/width.coerceAtLeast(1)
   val y=e.y/height.coerceAtLeast(1)
   buttons = when {
    x < .28f && y < .42f -> RogueInput.UP
    x < .28f && y > .68f -> RogueInput.DOWN
    x < .14f -> RogueInput.LEFT
    x < .38f -> RogueInput.RIGHT
    x > .82f -> RogueInput.A
    x > .64f -> RogueInput.B
    else -> 0
   }
  }
  if(nativeReady) runtime?.nativeSetButtons(buttons)
  return true
 }
 override fun onDraw(c:Canvas){
  super.onDraw(c)
  paint.color=Color.WHITE; paint.textSize=42f
  c.drawText("ROGUE 2.5D — Rogue Bridge",56f,72f,paint)
  paint.textSize=28f
  val error=startupError
  if(error != null) {
   c.drawText("Native startup error:",56f,118f,paint)
   c.drawText(error.take(80),56f,158f,paint)
  } else if(nativeReady) {
   c.drawText("Native tick: "+(runtime?.nativeTick() ?: -1),56f,118f,paint)
   c.drawText("Input mask: "+(runtime?.nativeButtons() ?: 0),56f,158f,paint)
  } else {
   c.drawText("Native runtime not started",56f,118f,paint)
  }
  paint.style=Paint.Style.STROKE; paint.strokeWidth=4f
  c.drawCircle(width*.18f,height*.72f,120f,paint)
  c.drawCircle(width*.82f,height*.70f,72f,paint)
  c.drawCircle(width*.68f,height*.78f,72f,paint)
  paint.style=Paint.Style.FILL
  c.drawText("A",width*.81f,height*.71f,paint)
  c.drawText("B",width*.67f,height*.79f,paint)
 }
}
