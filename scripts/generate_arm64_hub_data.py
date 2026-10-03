#!/usr/bin/env python3
import argparse
import json
import pathlib
import struct

def ctoken(value):
    if isinstance(value, bool):
        return "TRUE" if value else "FALSE"
    return str(value)

def read_u16_array(path):
    data = pathlib.Path(path).read_bytes()
    if len(data) % 2:
        raise SystemExit(f"{path}: odd byte count")
    return struct.unpack("<" + "H" * (len(data) // 2), data)

def emit_u16_array(name, values):
    lines=[f"static const u16 {name}[] = {{"]
    for i in range(0, len(values), 12):
        chunk=", ".join(f"0x{v:04X}" for v in values[i:i+12])
        lines.append("    " + chunk + ",")
    lines.append("};")
    return "\n".join(lines)

def main():
    ap=argparse.ArgumentParser()
    ap.add_argument("--core", required=True)
    ap.add_argument("--out", required=True)
    ap.add_argument("--map", default="Rogue_Hub")
    ap.add_argument("--layout-id", default="LAYOUT_ROGUE_HUB")
    args=ap.parse_args()

    core=pathlib.Path(args.core)
    map_path=core/"data/maps"/args.map/"map.json"
    layouts_path=core/"data/layouts/layouts.json"
    groups_path=core/"data/maps/map_groups.json"

    m=json.loads(map_path.read_text())
    layouts_doc=json.loads(layouts_path.read_text())
    groups=json.loads(groups_path.read_text())

    layout=next(x for x in layouts_doc["layouts"] if isinstance(x,dict) and x.get("id")==args.layout_id)
    layout_index=next(i for i,x in enumerate(layouts_doc["layouts"]) if isinstance(x,dict) and x.get("id")==args.layout_id)

    group_index=map_index=None
    group_name=None
    for gi,gname in enumerate(groups["group_order"]):
        arr=groups[gname]
        if args.map in arr:
            group_index=gi
            map_index=arr.index(args.map)
            group_name=gname
            break
    if group_index is None:
        raise SystemExit(f"map {args.map} not found in map_groups.json")

    border=read_u16_array(core/layout["border_filepath"])
    blockdata=read_u16_array(core/layout["blockdata_filepath"])

    scripts={f'{m["name"]}_MapScripts'}
    for obj in m.get("object_events",[]):
        s=obj.get("script")
        if s and s!="NULL":
            scripts.add(s)
    for bg in m.get("bg_events",[]):
        s=bg.get("script")
        if s and s!="NULL":
            scripts.add(s)
    for ce in m.get("coord_events",[]):
        s=ce.get("script")
        if s and s!="NULL":
            scripts.add(s)

    out=[]
    out += [
        '#include "global.h"',
        '#include "constants/event_bg.h"',
        '#include "constants/event_object_movement.h"',
        '#include "constants/event_objects.h"',
        '#include "constants/flags.h"',
        '#include "constants/layouts.h"',
        '#include "constants/maps.h"',
        '#include "constants/map_types.h"',
        '#include "constants/region_map_sections.h"',
        '#include "constants/songs.h"',
        '#include "constants/trainer_types.h"',
        '#include "constants/weather.h"',
        '',
        '/* Generated for Android ARM64 from the pinned upstream JSON/bin data.',
        ' * This preserves native pointer width instead of linking the upstream .4byte tables. */',
        f'extern const struct Tileset {layout["primary_tileset"]};',
        f'extern const struct Tileset {layout["secondary_tileset"]};',
    ]
    for s in sorted(scripts):
        out.append(f'extern const u8 {s}[];')
    out.append("")

    out.append(emit_u16_array("sRogueHubBorder", border))
    out.append("")
    out.append(emit_u16_array("sRogueHubBlockdata", blockdata))
    out.append("")

    out += [
        'const struct MapLayout Rogue_Hub_Layout = {',
        f'    .width = {layout["width"]},',
        f'    .height = {layout["height"]},',
        '    .border = sRogueHubBorder,',
        '    .map = sRogueHubBlockdata,',
        f'    .primaryTileset = &{layout["primary_tileset"]},',
        f'    .secondaryTileset = &{layout["secondary_tileset"]},',
        '};',
        ''
    ]

    objects=m.get("object_events",[])
    if objects:
        out.append("static const struct ObjectEventTemplate sRogueHubObjectEvents[] = {")
        for i,o in enumerate(objects,1):
            if o.get("type","object")!="object":
                raise SystemExit("Rogue_Hub contains unsupported non-object event")
            script=o.get("script","NULL")
            out += [
                "    {",
                f"        .localId = {i},",
                f"        .graphicsId = {ctoken(o['graphics_id'])},",
                f"        .x = {ctoken(o['x'])}, .y = {ctoken(o['y'])},",
                f"        .elevation = {ctoken(o['elevation'])},",
                f"        .movementType = {ctoken(o['movement_type'])},",
                f"        .movementRangeX = {ctoken(o['movement_range_x'])},",
                f"        .movementRangeY = {ctoken(o['movement_range_y'])},",
                f"        .trainerType = {ctoken(o['trainer_type'])},",
                f"        .trainerRange_berryTreeId = {ctoken(o['trainer_sight_or_berry_tree_id'])},",
                f"        .script = {script},",
                f"        .flagId = {ctoken(o['flag'])},",
                "    },"
            ]
        out += ["};",""]

    warps=m.get("warp_events",[])
    if warps:
        out.append("static const struct WarpEvent sRogueHubWarps[] = {")
        for w in warps:
            dest=w["dest_map"]
            map_token=dest[4:] if dest.startswith("MAP_") else dest
            out += [
                "    {",
                f"        .x = {ctoken(w['x'])}, .y = {ctoken(w['y'])},",
                f"        .elevation = {ctoken(w['elevation'])},",
                f"        .warpId = {ctoken(w['dest_warp_id'])},",
                f"        .mapNum = MAP_NUM({map_token}),",
                f"        .mapGroup = MAP_GROUP({map_token}),",
                "    },"
            ]
        out += ["};",""]

    bgs=m.get("bg_events",[])
    if bgs:
        out.append("static const struct BgEvent sRogueHubBgEvents[] = {")
        for b in bgs:
            if b.get("type")!="sign":
                raise SystemExit("Rogue_Hub contains unsupported non-sign bg event")
            out += [
                "    {",
                f"        .x = {ctoken(b['x'])}, .y = {ctoken(b['y'])},",
                f"        .elevation = {ctoken(b['elevation'])},",
                f"        .kind = {ctoken(b['player_facing_dir'])},",
                f"        .bgUnion.script = {ctoken(b['script'])},",
                "    },"
            ]
        out += ["};",""]

    out += [
        'const struct MapEvents Rogue_Hub_MapEvents = {',
        f'    .objectEventCount = {len(objects)},',
        f'    .warpCount = {len(warps)},',
        f'    .coordEventCount = {len(m.get("coord_events",[]))},',
        f'    .bgEventCount = {len(bgs)},',
        f'    .objectEvents = {"sRogueHubObjectEvents" if objects else "NULL"},',
        f'    .warps = {"sRogueHubWarps" if warps else "NULL"},',
        '    .coordEvents = NULL,',
        f'    .bgEvents = {"sRogueHubBgEvents" if bgs else "NULL"},',
        '};',
        '',
        'const struct MapHeader Rogue_Hub = {',
        '    .mapLayout = &Rogue_Hub_Layout,',
        '    .events = &Rogue_Hub_MapEvents,',
        f'    .mapScripts = {m["name"]}_MapScripts,',
        '    .connections = NULL,',
        f'    .music = {ctoken(m["music"])},',
        f'    .mapLayoutId = {args.layout_id},',
        f'    .regionMapSectionId = {ctoken(m["region_map_section"])},',
        f'    .cave = {ctoken(m["requires_flash"])},',
        f'    .weather = {ctoken(m["weather"])},',
        f'    .mapType = {ctoken(m["map_type"])},',
        f'    .allowCycling = {ctoken(m["allow_cycling"])},',
        f'    .allowEscaping = {ctoken(m["allow_escaping"])},',
        f'    .allowRunning = {ctoken(m["allow_running"])},',
        f'    .showMapName = {ctoken(m["show_map_name"])},',
        f'    .battleType = {ctoken(m["battle_scene"])},',
        '};',
        ''
    ]

    layout_count=len(layouts_doc["layouts"])
    out += [
        f'const struct MapLayout *const gMapLayouts[{layout_count}] = {{',
        f'    [{args.layout_id} - 1] = &Rogue_Hub_Layout,',
        '};',
        ''
    ]

    for gi,gname in enumerate(groups["group_order"]):
        size=max(1,len(groups[gname]))
        out.append(f'const struct MapHeader *const sAndroidMapGroup{gi}[{size}] = {{')
        if gi==group_index:
            out.append(f'    [{map_index}] = &Rogue_Hub,')
        out += ['};','']

    out.append(f'const struct MapHeader *const *const gMapGroups[{len(groups["group_order"])}] = {{')
    for gi,_ in enumerate(groups["group_order"]):
        out.append(f'    [{gi}] = sAndroidMapGroup{gi},')
    out += ['};','']

    out += [
        f'_Static_assert({args.layout_id} == {layout_index + 1}, "layout index mismatch");',
        f'_Static_assert(MAP_GROUP(ROGUE_HUB) == {group_index}, "map group mismatch");',
        f'_Static_assert(MAP_NUM(ROGUE_HUB) == {map_index}, "map number mismatch");',
        ''
    ]

    pathlib.Path(args.out).write_text("\n".join(out))

if __name__=="__main__":
    main()
