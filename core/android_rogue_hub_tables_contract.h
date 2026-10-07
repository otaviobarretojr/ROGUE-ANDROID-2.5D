#ifndef ROGUE25D_ANDROID_ROGUE_HUB_TABLES_CONTRACT_H
#define ROGUE25D_ANDROID_ROGUE_HUB_TABLES_CONTRACT_H

/*
 * Frozen upstream rogue.h defines the Hub layouts under this probe, but hides
 * these runtime table declarations when ROGUE_BAKING is enabled.
 */
extern const struct RogueHubArea gRogueHubAreas[];
extern const struct RogueAreaUpgrade gRogueHubUpgrades[];

#endif
