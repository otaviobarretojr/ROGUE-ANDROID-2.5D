#!/usr/bin/env python3
import argparse
import pathlib
import re

CPP_INCLUDES = [
    "config.h",
    "config/battle.h",
    "config/item.h",
    "constants/global.h",
    "constants/rogue.h",
    "constants/battle.h",
    "constants/berry.h",
    "constants/coins.h",
    "constants/decorations.h",
    "constants/event_objects.h",
    "constants/event_object_movement.h",
    "constants/field_effects.h",
    "constants/flags.h",
    "constants/item.h",
    "constants/items.h",
    "constants/layouts.h",
    "constants/map_scripts.h",
    "constants/maps.h",
    "constants/metatile_labels.h",
    "constants/moves.h",
    "constants/party_menu.h",
    "constants/pokemon.h",
    "constants/script_menu.h",
    "constants/songs.h",
    "constants/sound.h",
    "constants/species.h",
    "constants/vars.h",
    "constants/weather.h",
    "constants/follow_me.h",
]

LABEL = re.compile(r"^\s*([A-Za-z_][A-Za-z0-9_.$]*):{1,2}\s*(?:@.*)?$")

def labels_from(path):
    labels=[]
    for line in pathlib.Path(path).read_text().splitlines():
        m=LABEL.match(line)
        if m:
            labels.append(m.group(1))
    return labels

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--core", required=True)
    ap.add_argument("--out", required=True)
    args=ap.parse_args()

    core=pathlib.Path(args.core)
    hub=core/"data/maps/Rogue_Hub/scripts.inc"
    std=core/"data/scripts/std_msgbox.inc"
    if not hub.exists():
        raise SystemExit(f"missing generated {hub}")
    if not std.exists():
        raise SystemExit(f"missing {std}")

    labels=sorted(set(labels_from(hub)+labels_from(std)))

    out=[]
    for inc in CPP_INCLUDES:
        out.append(f'#include "{inc}"')
    out += [
        '',
        '\t.include "asm/macros.inc"',
        '\t.include "asm/macros/event.inc"',
        '\t.include "constants/constants.inc"',
        '',
        '\t.section script_data, "aw", %progbits',
        '\t.p2align 2',
        ''
    ]
    for label in labels:
        out.append(f'\t.global {label}')
    out += [
        '',
        '\t.include "data/scripts/std_msgbox.inc"',
        '\t.include "data/maps/Rogue_Hub/scripts.inc"',
        ''
    ]
    pathlib.Path(args.out).write_text("\n".join(out))

if __name__=="__main__":
    main()
