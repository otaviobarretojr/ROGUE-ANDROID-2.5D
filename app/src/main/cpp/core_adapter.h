#pragma once
#include <cstdint>

namespace rogue25d {

struct CoreCapabilities {
    bool portableCoreLinked;
    bool inputBridgeReady;
    bool storageBridgeReady;
    bool worldStateBridgeReady;
};

CoreCapabilities coreCapabilities();
void coreSetInput(std::uint32_t buttons);
void coreStep();

}
