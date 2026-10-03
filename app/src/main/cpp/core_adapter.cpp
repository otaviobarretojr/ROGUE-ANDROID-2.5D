#include "core_adapter.h"
#include "rogue_bridge.h"

#if defined(ROGUE_CORE_LINKED)
extern "C" {
void AgbMain(void);
void MainLoop(void);
void RunDMAsAndVBlank(void);
}
#endif

namespace rogue25d {
namespace {
bool sBooted=false;
}

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
    // Portable Rogue frame contract:
    // MainLoop() -> ReadKeys() -> Platform_GetKeyInput()
    // then commit DMA/VBlank work exactly once.
    MainLoop();
    RunDMAsAndVBlank();
#endif
}

}
