#include "android_platform.h"
#include "rogue_bridge.h"
#include <cstdio>
#include <cstring>
#include <string>

#if defined(ROGUE_CORE_LINKED)
extern "C" {
#include "gba/io_reg.h"
extern unsigned char FLASH_BASE[131072];
}
#endif

namespace {
std::string sSavePath;
}

extern "C" bool RogueAndroid_PlatformInit(const char* storagePath) {
    if(storagePath==nullptr || *storagePath=='\0') return false;
    sSavePath=std::string(storagePath)+"/pokeemerald.sav";
#if defined(ROGUE_CORE_LINKED)
    std::memset(FLASH_BASE,0xFF,131072);
    if(FILE* f=std::fopen(sSavePath.c_str(),"rb")) {
        std::fread(FLASH_BASE,1,131072,f);
        std::fclose(f);
    }
#endif
    return true;
}

extern "C" void RogueAndroid_PlatformShutdown(void) {
#if defined(ROGUE_CORE_LINKED)
    Platform_StoreSaveFile();
#endif
}

extern "C" std::uint16_t Platform_GetKeyInput(void) {
#if defined(ROGUE_CORE_LINKED)
    const auto b=rogue25d::bridge().buttons();
    std::uint16_t gba=0;
    using rogue25d::RogueButton;
    const auto has=[b](RogueButton button) {
        return (b & static_cast<std::uint32_t>(button)) != 0;
    };
    if(has(RogueButton::A))      gba |= A_BUTTON;
    if(has(RogueButton::B))      gba |= B_BUTTON;
    if(has(RogueButton::Select)) gba |= SELECT_BUTTON;
    if(has(RogueButton::Start))  gba |= START_BUTTON;
    if(has(RogueButton::Right))  gba |= DPAD_RIGHT;
    if(has(RogueButton::Left))   gba |= DPAD_LEFT;
    if(has(RogueButton::Up))     gba |= DPAD_UP;
    if(has(RogueButton::Down))   gba |= DPAD_DOWN;
    if(has(RogueButton::R))      gba |= R_BUTTON;
    if(has(RogueButton::L))      gba |= L_BUTTON;
    return gba;
#else
    return 0;
#endif
}

extern "C" void Platform_StoreSaveFile(void) {
#if defined(ROGUE_CORE_LINKED)
    if(sSavePath.empty()) return;
    if(FILE* f=std::fopen(sSavePath.c_str(),"wb")) {
        std::fwrite(FLASH_BASE,1,131072,f);
        std::fflush(f);
        std::fclose(f);
    }
#endif
}
