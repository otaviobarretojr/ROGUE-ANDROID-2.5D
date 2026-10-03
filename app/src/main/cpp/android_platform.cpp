#include "android_platform.h"
#include "rogue_bridge.h"
#include <cstdio>
#include <cstring>
#include <string>

#if defined(ROGUE_CORE_LINKED)
extern "C" unsigned char FLASH_BASE[131072];
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
    const auto b=rogue25d::bridge().buttons();
    // Our RogueButton bit layout intentionally matches the GBA logical order
    // used by the bridge. Explicit mapping to upstream constants is added
    // when upstream headers are part of the NDK target.
    return static_cast<std::uint16_t>(b & 0x03FFu);
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
