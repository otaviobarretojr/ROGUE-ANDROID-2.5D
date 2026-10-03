#include "global.h"
#include "constants/field_effects.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "sprite.h"
#include <string.h>

s32 gFieldEffectArguments[8] = {0};

static bool8 sActive[FLDEFF_COUNT];

u32 FieldEffectStart(u8 id)
{
    if (id < FLDEFF_COUNT)
        sActive[id] = TRUE;
    return 0;
}

bool8 FieldEffectActiveListContains(u8 id)
{
    return id < FLDEFF_COUNT ? sActive[id] : FALSE;
}

void FieldEffectActiveListClear(void)
{
    memset(sActive, 0, sizeof(sActive));
}

void FieldEffectActiveListAdd(u8 id)
{
    if (id < FLDEFF_COUNT)
        sActive[id] = TRUE;
}

void FieldEffectActiveListRemove(u8 id)
{
    if (id < FLDEFF_COUNT)
        sActive[id] = FALSE;
}

void FieldEffectStop(struct Sprite *sprite, u8 id)
{
    (void)sprite;
    FieldEffectActiveListRemove(id);
}

void FieldEffectFreeAllSprites(void)
{
    FieldEffectActiveListClear();
}

u8 CreateWarpArrowSprite(void)
{
    return MAX_SPRITES;
}

void ShowWarpArrowSprite(u8 spriteId, u8 direction, s16 x, s16 y)
{
    (void)spriteId;
    (void)direction;
    (void)x;
    (void)y;
}

u8 FindTallGrassFieldEffectSpriteId(
    u8 localId,
    u8 mapNum,
    u8 mapGroup,
    s16 x,
    s16 y)
{
    (void)localId;
    (void)mapNum;
    (void)mapGroup;
    (void)x;
    (void)y;
    return MAX_SPRITES;
}

u32 StartFieldEffectForObjectEvent(u8 id, struct ObjectEvent *objectEvent)
{
    (void)objectEvent;
    return FieldEffectStart(id);
}

void StartAshFieldEffect(s16 x, s16 y, u16 metatileId, s16 priority)
{
    (void)x;
    (void)y;
    (void)metatileId;
    (void)priority;
}

void SetUpReflection(struct ObjectEvent *objectEvent, struct Sprite *sprite, u8 mode)
{
    (void)objectEvent;
    (void)sprite;
    (void)mode;
}

void SetShadowFieldEffectVisible(struct ObjectEvent *objectEvent, bool8 state)
{
    (void)objectEvent;
    (void)state;
}
