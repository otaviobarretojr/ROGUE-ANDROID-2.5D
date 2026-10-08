#ifndef ROGUE25D_ANDROID_ROGUE_HUB_TABLES_CONTRACT_H
#define ROGUE25D_ANDROID_ROGUE_HUB_TABLES_CONTRACT_H

#include "constants/event_objects.h"

#ifndef OBJ_EVENT_GFX_FOLLOW_MON_0
#define OBJ_EVENT_GFX_FOLLOW_MON_0 (OBJ_EVENT_GFX_WILD_DEN_WATER + 2)
#endif

/*
 * Frozen upstream rogue.h defines the Hub layouts under this probe, but hides
 * these runtime table declarations when ROGUE_BAKING is enabled.
 */
extern const struct RogueHubArea gRogueHubAreas[];
extern const struct RogueAreaUpgrade gRogueHubUpgrades[];

#endif
