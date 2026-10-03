#include <jni.h>
#include "runtime.h"
extern "C" JNIEXPORT void JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeStart(JNIEnv*,jobject){ rogue25d::runtime().start(); }
extern "C" JNIEXPORT void JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeStop(JNIEnv*,jobject){ rogue25d::runtime().stop(); }
extern "C" JNIEXPORT void JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeStep(JNIEnv*,jobject,jdouble dt){ rogue25d::runtime().step(dt); }
extern "C" JNIEXPORT jlong JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeTick(JNIEnv*,jobject){ return static_cast<jlong>(rogue25d::runtime().snapshot().tick); }
