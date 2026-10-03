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

    # The source places one sentinel entry after gScriptCmdTableEnd.
    sentinel=handlers[-1]
    handlers=handlers[:-1]

    if len(handlers) != 0xF7:
        raise SystemExit(f"unexpected opcode count: {len(handlers)}")

    out=[
        '.section .rodata.android_script_cmds,"a",%progbits',
        '.p2align 3',
        '.global gScriptCmdTable',
        '.type gScriptCmdTable,%object',
        'gScriptCmdTable:',
    ]
    for name in handlers:
        out.append(f'    .xword {name}')

    out += [
        '.global gScriptCmdTableEnd',
        '.type gScriptCmdTableEnd,%object',
        'gScriptCmdTableEnd:',
        f'    .xword {sentinel}',
        ''
    ]

    pathlib.Path(args.out).write_text("\n".join(out))

if __name__=="__main__":
    main()
