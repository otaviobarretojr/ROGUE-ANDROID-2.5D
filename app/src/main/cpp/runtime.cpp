#include "runtime.h"
#include "android_platform.h"
#include "core_adapter.h"
#include "rogue_bridge.h"
#include <algorithm>

namespace rogue25d {
void Runtime::start(){
 if(running_.load(std::memory_order_acquire)) return;
 const auto& storagePath=bridge().storagePath();
 platformInitialized_=RogueAndroid_PlatformInit(storagePath.c_str());
 accumulatorSeconds_=0.0;
 running_.store(platformInitialized_, std::memory_order_release);
}

void Runtime::stop(){
 const bool wasRunning=running_.exchange(false, std::memory_order_acq_rel);
 if((wasRunning || platformInitialized_) && platformInitialized_) {
  RogueAndroid_PlatformShutdown();
  platformInitialized_=false;
 }
 accumulatorSeconds_=0.0;
}

void Runtime::step(double dt){
 if(!running_.load(std::memory_order_acquire)) return;
 // Choreographer may run at 90/120 Hz. Logic remains locked to 60 Hz;
 // rendering can later consume the fractional accumulator independently.
 dt=std::clamp(dt,0.0,0.25);
 accumulatorSeconds_+=dt;
 while(accumulatorSeconds_ >= kFixedStepSeconds) {
  coreStep();
  accumulatorSeconds_-=kFixedStepSeconds;
  elapsedSeconds_+=kFixedStepSeconds;
  ++tick_;
 }
}
RuntimeSnapshot Runtime::snapshot() const { return {tick_,elapsedSeconds_}; }
Runtime& runtime(){ static Runtime instance; return instance; }
}
