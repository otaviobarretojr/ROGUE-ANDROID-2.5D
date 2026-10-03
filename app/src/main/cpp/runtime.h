#pragma once
#include <atomic>
#include <cstdint>
namespace rogue25d {
struct RuntimeSnapshot { std::uint64_t tick; double elapsedSeconds; };
class Runtime {
public:
 void start(); void stop(); void step(double dt); RuntimeSnapshot snapshot() const;
private:
 static constexpr double kFixedStepSeconds = 1.0 / 60.0;
 std::atomic<bool> running_{false};
 bool platformInitialized_{false};
 std::uint64_t tick_{0};
 double elapsedSeconds_{0.0};
 double accumulatorSeconds_{0.0};
};
Runtime& runtime();
}
