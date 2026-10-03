#include "android_script_pointer.h"

extern const void *const gAndroidScriptPointerTable[];
extern const u64 gAndroidScriptPointerCount;

const void *AndroidScriptResolvePointer(u32 token)
{
    if (token == 0 || token >= gAndroidScriptPointerCount)
        return NULL;

    return gAndroidScriptPointerTable[token];
}
