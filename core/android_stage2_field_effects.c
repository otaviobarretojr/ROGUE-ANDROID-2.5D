#include "global.h"
#include "constants/field_effects.h"
#include "field_effect.h"
#include "field_effect_helpers.h"
#include "sprite.h"
#include <string.h>

s32 gFieldEffectArguments[8] = {0};

enum { ANDROID_STAGE2_FIELD_EFFECT_COUNT = FLDEFF_DOUBLE_EXCL_MARK_ICON + 1 };
static bool8 sActive[ANDROID_STAGE2_FIELD_EFFECT_COUNT];

u32 FieldEffectStart(u8 id)
{
    if (id < ANDROID_STAGE2_FIELD_EFFECT_COUNT)
        sActive[id] = TRUE;
    return 0;
}

bool8 FieldEffectActiveListContains(u8 id)
{
    return id < ANDROID_STAGE2_FIELD_EFFECT_COUNT ? sActive[id] : FALSE;
}

void FieldEffectActiveListClear(void)
{
    memset(sActive, 0, sizeof(sActive));
}

void FieldEffectActiveListAdd(u8 id)
{
    if (id < ANDROID_STAGE2_FIELD_EFFECT_COUNT)
        sActive[id] = TRUE;
}

void FieldEffectActiveListRemove(u8 id)
{
    if (id < ANDROID_STAGE2_FIELD_EFFECT_COUNT)
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


u8 StartUnderwaterSurfBlobBobbing(u8 oldSpriteId)
{
    return oldSpriteId;
}

void SetSurfBlob_BobState(u8 spriteId, u8 state)
{
    (void)spriteId;
    (void)state;
}

void SetSurfBlob_DontSyncAnim(u8 spriteId, bool8 dontSync)
{
    (void)spriteId;
    (void)dontSync;
}

void SetSurfBlob_PlayerOffset(u8 spriteId, bool8 hasOffset, s16 offset)
{
    (void)spriteId;
    (void)hasOffset;
    (void)offset;
}

bool8 UpdateRevealDisguise(struct ObjectEvent *objectEvent)
{
    (void)objectEvent;
    return FALSE;
}

void StartRevealDisguise(struct ObjectEvent *objectEvent)
{
    (void)objectEvent;
}

void UpdateRayquazaSpotlightEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateShadowFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateTallGrassFieldEffect(struct Sprite *sprite) { (void)sprite; }
void WaitFieldEffectSpriteAnim(struct Sprite *sprite) { (void)sprite; }
void UpdateAshFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateSurfBlobFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateJumpImpactEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateFootprintsTireTracksFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateSplashFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateLongGrassFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateSandPileFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateDisguiseFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateShortGrassFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateHotSpringsWaterFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateBubblesFieldEffect(struct Sprite *sprite) { (void)sprite; }
void UpdateSparkleFieldEffect(struct Sprite *sprite) { (void)sprite; }

void SetSpriteInvisible(u8 spriteId)
{
    if (spriteId < MAX_SPRITES)
        gSprites[spriteId].invisible = TRUE;
}
