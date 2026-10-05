#!/usr/bin/env python3
import pathlib
import re
import sys

CONTROL = {"l": [0xFA], "p": [0xFB], "n": [0xFE], "v": [0xFD]}
STRING = re.compile(r'^(\s*)\.string\s+"(.*)"\s*$')
CONSTANTS = {
    "STR_VAR_1": "0", "STR_VAR_2": "1", "STR_VAR_3": "2",
    "NO": "0", "YES": "1",
    "VARS_START": "0x4000", "VARS_END": "0x40FF",
    "SPECIAL_VARS_START": "0x8000", "SPECIAL_VARS_END": "0x8015",
    "VAR_0x8003": "0x8003", "VAR_0x8004": "0x8004", "VAR_0x8005": "0x8005",
    "VAR_RESULT": "0x800D", "VAR_ITEM_ID": "0x800E",
    "PARTY_NOTHING_CHOSEN": "0xFF", "MULTI_B_PRESSED": "127",
}

def convert(line):
    line = re.sub(r"^([A-Za-z_][A-Za-z0-9_.$]*)::", lambda m: m.group(1) + ":", line)

    # TM/HM foreach directives are compatibility alias generators, not Hub bytecode.
    if re.match(r"^\s*(?:FOREACH_(?:TM|HM)|\.macro\s+FOREACH_(?:TM|HM))\b", line):
        return []

    # Preserve assembler definitions such as YES = 1.
    is_definition = re.match(r"^\s*[A-Za-z_][A-Za-z0-9_]*\s*=", line) is not None
    if not is_definition:
        for name, value in CONSTANTS.items():
            line = re.sub(r"(?<![A-Za-z0-9_])" + re.escape(name) + r"(?![A-Za-z0-9_])", value, line)

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
    skipping_foreach_macro = False
    for line in src.read_text(encoding="utf-8", errors="surrogateescape").splitlines():
        if skipping_foreach_macro:
            if re.match(r"^\\s*\\.endm\\b", line):
                skipping_foreach_macro = False
            continue
        if re.search(r"\\.macro\\s+FOREACH_(?:TM|HM)\\b", line):
            skipping_foreach_macro = True
            continue
        out.extend(convert(line))
    dst.write_text("\n".join(out) + "\n", encoding="utf-8", errors="surrogateescape")

if __name__ == "__main__":
    main()
