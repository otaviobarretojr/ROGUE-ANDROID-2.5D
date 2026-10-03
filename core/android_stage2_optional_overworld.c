#include "global.h"
#include "constants/event_objects.h"
#include "field_player_avatar.h"
#include "event_object_movement.h"
#include "field_door.h"
#include "field_screen_effect.h"
#include "field_weather.h"
#include "overworld.h"
#include "sound.h"
#include "task.h"
#include "follow_me.h"
#include "rogue_followmon.h"
#include "rogue_multiplayer.h"
#include "rogue_player_customisation.h"
#include "rogue_ridemon.h"

/*
 * Optional overworld systems are intentionally inactive during Stage 2.
 * The Hub must first boot, move and process its own events without pulling
 * Pokémon/follow-mon/multiplayer/ride graphs. These functions preserve the
 * normal "feature inactive" semantics expected by the vanilla overworld.
 */

static const u8 sAndroidStage2EmptyName[] = { EOS };

bool8 PlayerHasFollower(void) { return FALSE; }
bool8 FollowerComingThroughDoor(void) { return FALSE; }
u8 GetFollowerObjectId(void) { return OBJECT_EVENTS_COUNT; }
u8 GetFollowerLocalId(void) { return 0; }
const u8 *GetFollowerScriptPointer(void) { return NULL; }
void CreateFollowerAvatar(void) {}
void FollowMe(struct ObjectEvent *npc, u8 state, bool8 ignoreScriptActive)
{
    (void)npc;
    (void)state;
    (void)ignoreScriptActive;
}
void FollowMe_BindToSurbBlobOnReloadScreen(void) {}
void FollowMe_HandleSprite(void) {}
bool8 FollowMe_IsCollisionExempt(struct ObjectEvent *obstacle, struct ObjectEvent *collider)
{
    (void)obstacle;
    (void)collider;
    return FALSE;
}
void FollowMe_TryRemoveFollowerOnWhiteOut(void) {}
void PrepareFollowerDismountSurf(void) {}

bool8 IsPlayerOnFoot(void)
{
    return (gPlayerAvatar.flags & PLAYER_AVATAR_FLAG_ON_FOOT) != 0;
}

void FollowMon_ClearCachedPartnerSpecies(void) {}
const u16 *FollowMon_GetGraphicsForPalSlot(u16 palSlot)
{
    (void)palSlot;
    return NULL;
}
bool8 FollowMon_IsMonObject(struct ObjectEvent *object, bool8 ignorePartnerMon)
{
    (void)object;
    (void)ignorePartnerMon;
    return FALSE;
}
void FollowMon_OverworldCB(void) {}
bool8 FollowMon_ShouldAlwaysAnimation(struct ObjectEvent *objectEvent)
{
    (void)objectEvent;
    return FALSE;
}
bool8 FollowMon_ShouldAnimationGrass(struct ObjectEvent *objectEvent)
{
    (void)objectEvent;
    return FALSE;
}
void SetupFollowParterMonObjectEvent(void) {}

u8 RogueMP_GetRemotePlayerId(void) { return 0; }
bool8 RogueMP_IsRemotePlayerActive(void) { return FALSE; }
const u8 *RogueMP_GetPlayerName(u8 playerId)
{
    (void)playerId;
    return sAndroidStage2EmptyName;
}

const struct ObjectEventGraphicsInfo *RogueNetPlayer_GetObjectEventGraphicsInfo(u8 state)
{
    return RoguePlayer_GetObjectEventGraphicsInfo(state);
}

bool8 Rogue_CanRideMonInvJumpLedge(void) { return FALSE; }
bool8 Rogue_CanRideMonSwim(void) { return FALSE; }
bool8 Rogue_IsActiveRideMonObject(u8 objectEventId)
{
    (void)objectEventId;
    return FALSE;
}
bool8 Rogue_IsRideMonFlying(void) { return FALSE; }
void Rogue_OnRideMonWarp(void) {}
void MovePlayerOnRideMon(u8 direction, u16 newKeys, u16 heldKeys)
{
    (void)direction;
    (void)newKeys;
    (void)heldKeys;
}
s16 RideMonGetPlayerSpeed(void) { return 1; }

void FollowMe_SetIndicatorToComeOutDoor(void) {}
void FollowMe_SetIndicatorToRecreateSurfBlob(void) {}
void FollowMe_WarpSetEnd(void) {}


void HideFollower(void)
{
}

void Task_DoDoorWarp(u8 taskId)
{
    struct Task *task = &gTasks[taskId];
    s16 *x = &task->data[2];
    s16 *y = &task->data[3];
    u8 playerObjId = gPlayerAvatar.objectEventId;

    switch (task->data[0])
    {
    case 0:
        if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_DASH))
            SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ON_FOOT);

        FreezeObjectEvents();
        PlayerGetDestCoords(x, y);
        PlaySE(GetDoorSoundEffect(*x, *y - 1));
        task->data[1] = FieldAnimateDoorOpen(*x, *y - 1);
        task->data[0] = 1;
        break;

    case 1:
        if (task->data[1] < 0 || !gTasks[task->data[1]].isActive)
        {
            ObjectEventClearHeldMovementIfActive(&gObjectEvents[playerObjId]);
            ObjectEventSetHeldMovement(&gObjectEvents[playerObjId], MOVEMENT_ACTION_WALK_NORMAL_UP);
            task->data[0] = 2;
        }
        break;

    case 2:
        if (IsPlayerStandingStill())
        {
            task->data[1] = FieldAnimateDoorClose(*x, *y - 1);
            ObjectEventClearHeldMovementIfFinished(&gObjectEvents[playerObjId]);
            SetPlayerVisibility(FALSE);
            task->data[0] = 3;
        }
        break;

    case 3:
        if (task->data[1] < 0 || !gTasks[task->data[1]].isActive)
            task->data[0] = 4;
        break;

    case 4:
        TryFadeOutOldMapMusic();
        WarpFadeOutScreen();
        PlayRainStoppingSoundEffect();
        task->data[0] = 0;
        task->func = Task_WarpAndLoadMap;
        break;
    }
}
