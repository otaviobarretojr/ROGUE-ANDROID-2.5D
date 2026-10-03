#include "global.h"
#include "constants/event_objects.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "event_data.h"
#include "rogue_controller.h"
#include "rogue_hub.h"

/*
 * Stage 2 controller bridge.
 *
 * The full rogue_controller.c mixes Hub hooks with adventure, battle, ride,
 * follow-mon, multiplayer and encounter systems. For the minimal Android Hub
 * graph we preserve the authoritative Hub mutations and keep the not-yet-linked
 * systems inactive. This file is removed once the full controller graph is safe
 * to link.
 */

bool8 Rogue_IsRunActive(void)
{
    return FlagGet(FLAG_ROGUE_RUN_ACTIVE);
}

u8 Rogue_GetOverworldSpeedScale(void)
{
    return 1;
}

u16 Rogue_ModifyPlayBGM(u16 songNum)
{
    return songNum;
}

bool8 Rogue_ModifyPaletteDecompress(const u32 *input, void *writeBuffer)
{
    (void)input;
    (void)writeBuffer;
    return FALSE;
}

bool8 Rogue_ModifyObjectPaletteSlot(u16 graphicsId, u8 *palSlot)
{
    (void)graphicsId;
    (void)palSlot;
    return FALSE;
}

void Rogue_ModifyOverworldPalette(u16 offset, u16 count)
{
    (void)offset;
    (void)count;
}

const u8 *Rogue_ModifyOverworldInteractionScript(
    struct MapPosition *position,
    u16 metatileBehavior,
    u8 direction,
    const u8 *script)
{
    (void)position;
    (void)metatileBehavior;
    (void)direction;
    return script;
}

const struct Tileset *Rogue_ModifyOverworldTileset(const struct Tileset *tileset)
{
    if (Rogue_IsRunActive())
        return tileset;

    return RogueHub_ModifyOverworldTileset(tileset);
}

bool8 Rogue_IsObjectEventExcludedFromSave(struct ObjectEvent *objectEvent)
{
    if (objectEvent == NULL)
        return FALSE;

    return objectEvent->localId >= OBJ_EVENT_ID_MULTIPLAYER_FIRST
        && objectEvent->localId <= OBJ_EVENT_ID_MULTIPLAYER_LAST;
}

bool8 Rogue_OnProcessPlayerFieldInput(void)
{
    return FALSE;
}

void Rogue_MainInit(void) {}
void Rogue_MainEarlyCB(void) {}
void Rogue_MainLateCB(void) {}

void Rogue_OverworldCB(u16 newKeys, u16 heldKeys, bool8 inputActive)
{
    (void)newKeys;
    (void)heldKeys;
    (void)inputActive;
}

void Rogue_OnReturnToField(void)
{
    if (!Rogue_IsRunActive())
        RogueHub_ReloadObjectsAndTiles();
}

bool8 Rogue_IsCollisionExempt(struct ObjectEvent *obstacle, struct ObjectEvent *collider)
{
    (void)obstacle;
    (void)collider;
    return FALSE;
}

bool8 Rogue_IsRunningToggledOn(void)
{
    return FALSE;
}

void Rogue_OnSpawnObjectEvent(struct ObjectEvent *objectEvent, u8 objectEventId)
{
    (void)objectEvent;
    (void)objectEventId;
}

void Rogue_OnRemoveObjectEvent(struct ObjectEvent *objectEvent)
{
    (void)objectEvent;
}

void Rogue_OnObjectEventMovement(u8 objectEventId)
{
    (void)objectEventId;
}

void Rogue_OnResumeMap(void) {}

void Rogue_OnObjectEventsInit(void) {}

bool8 Rogue_TryGetCachedObjectEventId(u32 localId, u8 *eventObjectId)
{
    (void)localId;
    if (eventObjectId != NULL)
        *eventObjectId = OBJECT_EVENTS_COUNT;
    return FALSE;
}

void Rogue_OnLoadMap(void)
{
    if (!Rogue_IsRunActive())
    {
        RogueHub_UpdateWarpStates();
        RogueHub_ApplyMapMetatiles();
    }
}

bool8 Rogue_ShouldSkipReloadMapTileView(void)
{
    return !Rogue_IsRunActive();
}

void Rogue_OnWarpIntoMap(void) {}

void Rogue_OnSetWarpData(struct WarpData *warp)
{
    (void)warp;
}

void Rogue_ModifyMapHeader(struct MapHeader *mapHeader)
{
    (void)mapHeader;
}

void Rogue_ModifyMapWarpEvent(struct MapHeader *mapHeader, u8 warpId, struct WarpEvent *warp)
{
    RogueHub_ModifyMapWarpEvent(mapHeader, warpId, warp);
}

bool8 Rogue_AcceptMapConnection(struct MapHeader *mapHeader, const struct MapConnection *connection)
{
    return RogueHub_AcceptMapConnection(mapHeader, connection);
}

void Rogue_ModifyObjectEvents(
    struct MapHeader *mapHeader,
    bool8 loadingFromSave,
    struct ObjectEventTemplate *objectEvents,
    u8 *objectEventCount,
    u8 objectEventCapacity)
{
    if (mapHeader == NULL || objectEvents == NULL || objectEventCount == NULL)
        return;

    if (!Rogue_IsRunActive() && RogueHub_IsPlayerBaseLayout(mapHeader->mapLayoutId))
    {
        RogueHub_ModifyPlayerBaseObjectEvents(
            mapHeader->mapLayoutId,
            loadingFromSave,
            objectEvents,
            objectEventCount,
            objectEventCapacity);
    }
}
