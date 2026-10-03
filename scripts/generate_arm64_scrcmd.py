#!/usr/bin/env python3
import argparse
import pathlib

REPLACEMENTS = {
    "(bool8 (*)(void))ScriptReadWord(ctx)": "(bool8 (*)(void))AndroidScriptReadPointer(ctx)",
    "(NativeFunc)ScriptReadWord(ctx)": "(NativeFunc)AndroidScriptReadPointer(ctx)",
    "(const u8 *)ScriptReadWord(ctx)": "(const u8 *)AndroidScriptReadPointer(ctx)",
    "(const u8*) ScriptReadWord(ctx)": "(const u8*) AndroidScriptReadPointer(ctx)",
    "(u8 *)ScriptReadWord(ctx)": "(u8 *)AndroidScriptReadPointer(ctx)",
    "(u8*) ScriptReadWord(ctx)": "(u8*) AndroidScriptReadPointer(ctx)",
    "(void *)ScriptReadWord(ctx)": "(void *)AndroidScriptReadPointer(ctx)",
    "(const void *)ScriptReadWord(ctx)": "(const void *)AndroidScriptReadPointer(ctx)",
    "(const u8 *)ctx->data[0]": "(const u8 *)AndroidScriptResolvePointer(ctx->data[0])",
}

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--core", required=True)
    ap.add_argument("--out", required=True)
    args=ap.parse_args()

    src_path=pathlib.Path(args.core)/"src/scrcmd.c"
    text=src_path.read_text()

    for old,new in REPLACEMENTS.items():
        if old not in text:
            raise SystemExit(f"expected ARM64 script pointer pattern missing: {old}")
        text=text.replace(old,new)

    # Virtual-address opcodes encode absolute 32-bit addresses and cannot be
    # represented safely on an ASLR'd ARM64 process. They are not emitted by
    # the Stage 2 Hub pory scripts. Stop them cleanly if encountered instead
    # of truncating a host pointer.
    start=text.find("bool8 ScrCmd_setvaddress(struct ScriptContext *ctx)")
    end=text.find("bool8 ScrCmd_loadword(struct ScriptContext *ctx)", start)
    if start < 0 or end < 0:
        raise SystemExit("virtual script command block not found")
    replacement=r'''bool8 ScrCmd_setvaddress(struct ScriptContext *ctx)
{
    (void)ScriptReadWord(ctx);
    ctx->mode = SCRIPT_MODE_STOPPED;
    return TRUE;
}

bool8 ScrCmd_vgoto(struct ScriptContext *ctx)
{
    (void)ScriptReadWord(ctx);
    ctx->mode = SCRIPT_MODE_STOPPED;
    return TRUE;
}

bool8 ScrCmd_vcall(struct ScriptContext *ctx)
{
    (void)ScriptReadWord(ctx);
    ctx->mode = SCRIPT_MODE_STOPPED;
    return TRUE;
}

bool8 ScrCmd_vgoto_if(struct ScriptContext *ctx)
{
    (void)ScriptReadByte(ctx);
    (void)ScriptReadWord(ctx);
    ctx->mode = SCRIPT_MODE_STOPPED;
    return TRUE;
}

bool8 ScrCmd_vcall_if(struct ScriptContext *ctx)
{
    (void)ScriptReadByte(ctx);
    (void)ScriptReadWord(ctx);
    ctx->mode = SCRIPT_MODE_STOPPED;
    return TRUE;
}

'''
    text=text[:start]+replacement+text[end:]

    # The remaining virtual message/buffer opcodes use the same obsolete
    # address-offset encoding. Stage 2 Hub does not emit them.
    virtual_funcs = [
        "ScrCmd_vmessage",
        "ScrCmd_vbuffermessage",
        "ScrCmd_vbufferstring",
    ]
    for name in virtual_funcs:
        marker=f"bool8 {name}(struct ScriptContext *ctx)"
        pos=text.find(marker)
        if pos < 0:
            raise SystemExit(f"{name} not found")
        brace=text.find("{",pos)
        depth=0
        i=brace
        while i < len(text):
            if text[i]=="{": depth+=1
            elif text[i]=="}":
                depth-=1
                if depth==0:
                    i+=1
                    break
            i+=1
        body=marker+"\n{\n    (void)ctx;\n    return FALSE;\n}"
        text=text[:pos]+body+text[i:]

    text='#include "android_script_pointer.h"\n'+text
    pathlib.Path(args.out).write_text(text)

if __name__=="__main__":
    main()
