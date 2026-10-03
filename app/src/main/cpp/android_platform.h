#pragma once
#include <cstdint>

extern "C" {

// Implemented by Android adapter when ROGUE_CORE_LINKED is enabled.
std::uint16_t Platform_GetKeyInput(void);
void Platform_StoreSaveFile(void);

// Lifecycle exposed to the Rogue adapter.
bool RogueAndroid_PlatformInit(const char* storagePath);
void RogueAndroid_PlatformShutdown(void);

}
