#include "global.h"
#include "rogue_save.h"
#include "pokemon_storage_system.h"

struct RogueSaveBlock *gRogueSaveBlock = NULL;

enum
{
    ANDROID_STAGE2_ROGUE_SAVE_FORMAT_UNKNOWN,
    ANDROID_STAGE2_ROGUE_SAVE_FORMAT_READ,
    ANDROID_STAGE2_ROGUE_SAVE_FORMAT_WRITE,
};

void RogueSave_UpdatePointers(void)
{
    void *ptr = &gPokemonStoragePtr->boxes[TOTAL_BOXES_COUNT][0];
    gRogueSaveBlock = (struct RogueSaveBlock *)ptr;
}

void RogueSave_FormatForWriting(void)
{
    if (gRogueSaveBlock == NULL)
        return;

    gRogueSaveBlock->saveVersion = ROGUE_SAVE_VERSION;
    gRogueSaveBlock->currentBlockFormat = ANDROID_STAGE2_ROGUE_SAVE_FORMAT_WRITE;
}

void RogueSave_FormatForReading(void)
{
    if (gRogueSaveBlock == NULL)
        return;

    gRogueSaveBlock->currentBlockFormat = ANDROID_STAGE2_ROGUE_SAVE_FORMAT_READ;
}

void RogueSave_OnSaveLoaded(void)
{
    /*
     * Adventure/party/bag migration stays disabled until those systems join
     * the graph. The Rogue save block itself remains at its canonical storage
     * location so Hub-owned state can be addressed correctly.
     */
}
