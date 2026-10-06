#ifndef ROGUE25D_ANDROID_ROGUE_HUB_TABLES_CONTRACT_H
#define ROGUE25D_ANDROID_ROGUE_HUB_TABLES_CONTRACT_H

/*
 * Runtime table declarations from frozen upstream include/rogue.h.
 * They are hidden there under ROGUE_BAKING, which the ARM64 compatibility
 * probe intentionally uses to keep unrelated baked-data dependencies out.
 */
extern const struct RogueHubArea gRogueHubAreas[];
extern const struct RogueAreaUpgrade gRogueHubUpgrades[];

#endif
