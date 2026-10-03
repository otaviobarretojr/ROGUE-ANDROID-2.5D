package com.otaviobarreto.rogue25d
class NativeRuntime {
 companion object { init { System.loadLibrary("rogue25d") } }
 external fun nativeStart()
 external fun nativeStop()
 external fun nativeStep(dt: Double)
 external fun nativeTick(): Long
}
