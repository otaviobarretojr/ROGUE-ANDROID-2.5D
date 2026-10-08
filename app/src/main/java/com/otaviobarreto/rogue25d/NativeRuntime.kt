package com.otaviobarreto.rogue25d
class NativeRuntime {
 companion object {
  @Volatile private var loaded=false
  @Synchronized fun load():Result<Unit> {
   if(loaded) return Result.success(Unit)
   return runCatching {
    System.loadLibrary("rogue25d")
    loaded=true
   }
  }
 }
 external fun nativeStart()
 external fun nativeStop()
 external fun nativeStep(dt: Double)
 external fun nativeTick(): Long
 external fun nativeSetStoragePath(path: String)
 external fun nativeSetButtons(buttons: Int)
 external fun nativeButtons(): Int
}
