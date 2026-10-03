#include "core_adapter.h"
#include "rogue_bridge.h"

#if defined(ROGUE_CORE_LINKED)
extern "C" {
#include "platform/system.h"
void AgbMain(void);
void MainLoop(void);
void RunDMAsAndVBlank(void);
}
#endif

namespace rogue25d {
#if defined(ROGUE_CORE_LINKED)
namespace {
bool sBooted=false;
}
#endif

CoreCapabilities coreCapabilities() {
#if defined(ROGUE_CORE_LINKED)
    constexpr bool linked=true;
#else
    constexpr bool linked=false;
#endif
    return {linked,true,true,true};
}

void coreSetInput(std::uint32_t buttons) {
    bridge().setButtons(buttons);
}

void coreStep() {
#if defined(ROGUE_CORE_LINKED)
    if(!sBooted) {
        AgbMain();
        sBooted=true;
    }
    // Match the portable SDL frame contract exactly: enter VBlank before
    // advancing game logic, then commit DMA/VBlank work once for this tick.
    ENTER_VBLANK();
    MainLoop();
    RunDMAsAndVBlank();
#endif
}

}
