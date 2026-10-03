#include "global.h"
#include "event_data.h"

u16 gSpecialVar_ItemId = 0;
u16 gTrainerBattleOpponent_A = 0;

u16 *const gSpecialVars[] =
{
    &gSpecialVar_0x8000,
    &gSpecialVar_0x8001,
    &gSpecialVar_0x8002,
    &gSpecialVar_0x8003,
    &gSpecialVar_0x8004,
    &gSpecialVar_0x8005,
    &gSpecialVar_0x8006,
    &gSpecialVar_0x8007,
    &gSpecialVar_0x8008,
    &gSpecialVar_0x8009,
    &gSpecialVar_0x800A,
    &gSpecialVar_0x800B,
    &gSpecialVar_Facing,
    &gSpecialVar_Result,
    &gSpecialVar_ItemId,
    &gSpecialVar_LastTalked,
    &gSpecialVar_Unused_0x8010,
    &gSpecialVar_Unused_0x8011,
    &gSpecialVar_MonBoxId,
    &gSpecialVar_MonBoxPos,
    &gSpecialVar_Unused_0x8014,
    &gTrainerBattleOpponent_A,
};

_Static_assert(ARRAY_COUNT(gSpecialVars) == 22, "special var table mismatch");
