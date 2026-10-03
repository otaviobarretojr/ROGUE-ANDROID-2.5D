#include "core_adapter.h"
#include "rogue_bridge.h"

namespace rogue25d {

CoreCapabilities coreCapabilities() {
#ifdef ROGUE_CORE_LINKED
    constexpr bool linked=true;
#else
    constexpr bool linked=false;
#endif
    return {linked,true,true,true};
}

void coreSetInput(std::uint32_t buttons) {
    bridge().setButtons(buttons);
    // P2: map this mask to the portable core key state.
}

void coreStep() {
    // P2 integration seam. The real Rogue frame will be invoked here.
}

}
