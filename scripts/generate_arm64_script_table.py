#!/usr/bin/env python3
import argparse
import pathlib
import re

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--core", required=True)
    ap.add_argument("--out", required=True)
    args=ap.parse_args()

    core=pathlib.Path(args.core)
    src=(core/"data/script_cmd_table.inc").read_text()
    handlers=re.findall(r"^\s*\.4byte\s+(ScrCmd_[A-Za-z0-9_]+)", src, re.M)
    if not handlers:
        raise SystemExit("no script handlers found")

    # Last .4byte belongs to gScriptCmdTableEnd sentinel, not opcode table.
    sentinel=handlers[-1]
    handlers=handlers[:-1]

    out=[
        '#include "global.h"',
        '#include "script.h"',
        '',
        '/* Generated from pinned upstream data/script_cmd_table.inc.',
        ' * Native function pointers are required on Android ARM64. */',
    ]
    for name in sorted(set(handlers+[sentinel])):
        out.append(f'extern bool8 {name}(struct ScriptContext *ctx);')

    out += [
        '',
        f'ScrCmdFunc gScriptCmdTable[{len(handlers)}] = {{',
    ]
    for i,name in enumerate(handlers):
        out.append(f'    [{i:#04x}] = {name},')
    out += [
        '};',
        '',
        '/* script.c treats this symbol address as the one-past-end marker. */',
        f'ScrCmdFunc gScriptCmdTableEnd[1] = {{ {sentinel} }};',
        '',
        f'_Static_assert(ARRAY_COUNT(gScriptCmdTable) == {len(handlers)}, "script opcode count mismatch");',
        ''
    ]
    pathlib.Path(args.out).write_text("\n".join(out))

if __name__=="__main__":
    main()
