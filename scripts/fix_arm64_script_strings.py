#!/usr/bin/env python3
import pathlib
import re
import sys

CONTROL = {"l": [0xFA], "p": [0xFB], "n": [0xFE], "v": [0xFD]}
STRING = re.compile(r'^(\s*)\.string\s+"(.*)"\s*$')

def convert(line):
    m = STRING.match(line)
    if not m:
        return [line]
    indent, body = m.groups()
    parts = re.split(r'(\\[lpnv])', body)
    out = []
    for part in parts:
        if not part:
            continue
        if len(part) == 2 and part[0] == "\\" and part[1] in CONTROL:
            out.append(f"{indent}.byte " + ",".join(f"0x{b:02X}" for b in CONTROL[part[1]]))
        else:
            out.append(f'{indent}.string "{part}"')
    return out

def main():
    src, dst = map(pathlib.Path, sys.argv[1:3])
    out = []
    for line in src.read_text(encoding="utf-8", errors="surrogateescape").splitlines():
        out.extend(convert(line))
    dst.write_text("\n".join(out) + "\n", encoding="utf-8", errors="surrogateescape")

if __name__ == "__main__":
    main()
