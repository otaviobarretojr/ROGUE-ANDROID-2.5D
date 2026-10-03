#pragma once
#include <cstdint>

extern "C" {

// Implemented by Android adapter when ROGUE_CORE_LINKED is enabled.
std::uint16_t Platform_GetKeyInput(void);
void Platform_StoreSaveFile(void);
void Platform_ReadFlash(std::uint16_t sectorNum, std::uint32_t offset, std::uint8_t* dest, std::uint32_t size);

// Lifecycle exposed to the Rogue adapter.
bool RogueAndroid_PlatformInit(const char* storagePath);
void RogueAndroid_PlatformShutdown(void);

}
