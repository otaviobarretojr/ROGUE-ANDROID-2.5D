#include "global.h"

extern const u8 Std_MsgboxNPC[];
extern const u8 Std_MsgboxSign[];
extern const u8 Std_MsgboxDefault[];
extern const u8 Std_MsgboxYesNo[];
extern const u8 Std_MsgboxGetPoints[];
extern const u8 Std_MsgboxPokenav[];

const u8 *gStdScripts[] =
{
    [0] = NULL,                  /* STD_OBTAIN_ITEM */
    [1] = NULL,                  /* STD_FIND_ITEM */
    [2] = Std_MsgboxNPC,
    [3] = Std_MsgboxSign,
    [4] = Std_MsgboxDefault,
    [5] = Std_MsgboxYesNo,
    [6] = NULL,                  /* STD_MSGBOX_AUTOCLOSE */
    [7] = NULL,                  /* STD_OBTAIN_DECORATION */
    [8] = NULL,                  /* STD_REGISTER_MATCH_CALL */
    [9] = Std_MsgboxGetPoints,
    [10] = Std_MsgboxPokenav,
};

const u32 gStdScriptsCount = ARRAY_COUNT(gStdScripts);
