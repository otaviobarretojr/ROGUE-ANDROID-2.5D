#include "runtime.h"
#include <algorithm>
namespace rogue25d {
void Runtime::start(){ running_.store(true, std::memory_order_release); }
void Runtime::stop(){ running_.store(false, std::memory_order_release); }
void Runtime::step(double dt){
 if(!running_.load(std::memory_order_acquire)) return;
 dt=std::clamp(dt,0.0,0.1); elapsedSeconds_+=dt; ++tick_;
}
RuntimeSnapshot Runtime::snapshot() const { return {tick_,elapsedSeconds_}; }
Runtime& runtime(){ static Runtime instance; return instance; }
}
