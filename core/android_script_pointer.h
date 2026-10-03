#pragma once
#include "global.h"
#include "script.h"

const void *AndroidScriptResolvePointer(u32 token);
static inline const void *AndroidScriptReadPointer(struct ScriptContext *ctx)
{
    return AndroidScriptResolvePointer(ScriptReadWord(ctx));
}
