#include <jni.h>
#include <string>
#include "core_adapter.h"
#include "runtime.h"
#include "rogue_bridge.h"

extern "C" JNIEXPORT void JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeStart(JNIEnv*,jobject){ rogue25d::runtime().start(); }
extern "C" JNIEXPORT void JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeStop(JNIEnv*,jobject){ rogue25d::runtime().stop(); }
extern "C" JNIEXPORT void JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeStep(JNIEnv*,jobject,jdouble dt){ rogue25d::runtime().step(dt); }
extern "C" JNIEXPORT jlong JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeTick(JNIEnv*,jobject){ return static_cast<jlong>(rogue25d::runtime().snapshot().tick); }

extern "C" JNIEXPORT void JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeSetStoragePath(JNIEnv* env,jobject,jstring value){
 const char* chars=env->GetStringUTFChars(value,nullptr);
 rogue25d::bridge().setStoragePath(chars ? chars : "");
 if(chars) env->ReleaseStringUTFChars(value,chars);
}
extern "C" JNIEXPORT void JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeSetButtons(JNIEnv*,jobject,jint buttons){
 rogue25d::coreSetInput(static_cast<std::uint32_t>(buttons));
}
extern "C" JNIEXPORT jint JNICALL Java_com_otaviobarreto_rogue25d_NativeRuntime_nativeButtons(JNIEnv*,jobject){
 return static_cast<jint>(rogue25d::bridge().buttons());
}
