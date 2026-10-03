#include "global.h"
#include "constants/event_objects.h"
#include "rogue_player_customisation.h"

/*
 * Minimal Android Stage 2 player presentation provider.
 *
 * Player customisation is presentation/UI-heavy and references every outfit.
 * Until that asset layer is moved behind the Android renderer, keep one
 * canonical avatar while preserving the upstream ObjectEventGraphicsInfo ABI.
 */
extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_PlayerBrendanNormal;
extern const struct ObjectEventGraphicsInfo gObjectEventGraphicsInfo_PlayerBrendanRiding;

enum
{
    ANDROID_STAGE2_OUTFIT_BRENDAN = 4,
};

u16 RoguePlayer_GetOutfitId(void)
{
    return ANDROID_STAGE2_OUTFIT_BRENDAN;
}

bool8 RoguePlayer_HasSpritingAnim(void)
{
    return TRUE;
}

const struct ObjectEventGraphicsInfo* RoguePlayer_GetObjectEventGraphicsInfo(u8 state)
{
    switch (state)
    {
    case PLAYER_AVATAR_STATE_RIDE_GRABBING:
        return &gObjectEventGraphicsInfo_PlayerBrendanRiding;
    case PLAYER_AVATAR_STATE_NORMAL:
    case PLAYER_AVATAR_STATE_FIELD_MOVE:
    default:
        return &gObjectEventGraphicsInfo_PlayerBrendanNormal;
    }
}
