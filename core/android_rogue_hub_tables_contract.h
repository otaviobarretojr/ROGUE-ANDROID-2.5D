#ifndef ROGUE25D_ANDROID_ROGUE_HUB_TABLES_CONTRACT_H
#define ROGUE25D_ANDROID_ROGUE_HUB_TABLES_CONTRACT_H

#include "constants/rogue_hub.h"

/*
 * Minimal runtime Hub ABI copied from frozen upstream include/rogue.h.
 * ROGUE_BAKING hides both these layouts and their table declarations, while
 * the Android ARM64 compatibility probe still compiles src/rogue_hub.c.
 */
struct RogueHubArea
{
    const u32* iconImage;
    const u32* iconPalette;
    const u8* descText;
    const u8 areaName[ITEM_NAME_LENGTH];
    u8 connectionWarps[6][2];
    u8 requiredUpgrades[HUB_UPGRADE_MAX_REQUIREMENTS];
    u16 primaryMapNum;
    u16 primaryMapLayout;
    u8 primaryMapGroup;
    u8 buildCost;
} GBA_STRUCT_LAYOUT;

struct RogueAreaUpgrade
{
    const u32* iconImage;
    const u32* iconPalette;
    const u8* descText;
    const u8 upgradeName[ITEM_NAME_LENGTH];
    u8 requiredUpgrades[HUB_UPGRADE_MAX_REQUIREMENTS];
    u8 targetArea;
    u8 buildCost;
    bool8 isHidden : 1;
} GBA_STRUCT_LAYOUT;

extern const struct RogueHubArea gRogueHubAreas[];
extern const struct RogueAreaUpgrade gRogueHubUpgrades[];

#endif
