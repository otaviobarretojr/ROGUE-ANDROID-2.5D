#include "global.h"
#include "link.h"
#include "save_location.h"
#include "secret_base.h"
#include "trainer_hill.h"

u32 *gTrainerHillVBlankCounter = NULL;

void LoadTrainerHillObjectEventTemplates(void) {}
bool32 LoadTrainerHillFloorObjectEventScripts(void) { return FALSE; }
void GenerateTrainerHillFloorLayout(u16 *mapArg) { (void)mapArg; }
bool32 InTrainerHill(void) { return FALSE; }
u8 GetCurrentTrainerHillMapId(void) { return 0; }
const struct WarpEvent *SetWarpDestinationTrainerHill4F(void) { return NULL; }
const struct WarpEvent *SetWarpDestinationTrainerHillFinalFloor(u8 warpEventId)
{
    (void)warpEventId;
    return NULL;
}
const u8 *GetTrainerHillTrainerScript(void) { return NULL; }
u8 GetNumFloorsInTrainerHillChallenge(void) { return 0; }
void TryLoadTrainerHillEReaderPalette(void) {}
bool32 OnTrainerHillEReaderChallengeFloor(void) { return FALSE; }

void SetOccupiedSecretBaseEntranceMetatiles(const struct MapEvents *events)
{
    (void)events;
}
void InitSecretBaseAppearance(bool8 hidePC) { (void)hidePC; }
bool8 CurMapIsSecretBase(void) { return FALSE; }
void SecretBasePerStepCallback(u8 taskId) { (void)taskId; }
bool8 TrySetCurSecretBase(void) { return FALSE; }
void CheckInteractedWithFriendsPosterDecor(void) {}
void CheckInteractedWithFriendsFurnitureBottom(void) {}
void CheckInteractedWithFriendsFurnitureMiddle(void) {}
void CheckInteractedWithFriendsFurnitureTop(void) {}
bool8 SecretBaseMapPopupEnabled(void) { return FALSE; }
void CheckLeftFriendsSecretBase(void) {}

void TrySetMapSaveWarpStatus(void) {}

struct Link gLink = {0};
u16 ALIGNED(4) gRecvCmds[MAX_RFU_PLAYERS][CMD_LENGTH] = {0};
u8 gBlockSendBuffer[BLOCK_BUFFER_SIZE] = {0};
u16 gLinkType = 0;
u32 gLinkStatus = 0;
u16 gBlockRecvBuffer[MAX_RFU_PLAYERS][BLOCK_BUFFER_SIZE / 2] = {0};
u16 gSendCmd[CMD_LENGTH] = {0};
struct LinkPlayer gLinkPlayers[MAX_RFU_PLAYERS] = {0};
bool8 gReceivedRemoteLinkPlayers = FALSE;
u16 gLinkPartnersHeldKeys[6] = {0};
bool8 gLinkVSyncDisabled = FALSE;

void CloseLink(void) {}
bool8 IsLinkTaskFinished(void) { return TRUE; }
void SetLinkStandbyCallback(void) {}
void SetCloseLinkCallback(void) {}
bool8 HandleLinkConnection(void) { return FALSE; }
bool32 InUnionRoom(void) { return FALSE; }
bool32 IsSendingKeysToLink(void) { return FALSE; }
u32 GetLinkRecvQueueLength(void) { return 0; }
u8 GetLinkPlayerCount(void) { return 1; }
u8 GetLinkPlayerCount_2(void) { return 1; }
bool8 IsLinkConnectionEstablished(void) { return FALSE; }
bool8 HasLinkErrorOccurred(void) { return FALSE; }
bool8 IsWirelessAdapterConnected(void) { return FALSE; }
void LoadWirelessStatusIndicatorSpriteGfx(void) {}
void CreateWirelessStatusIndicatorSprite(u8 x, u8 y)
{
    (void)x;
    (void)y;
}
