# Architecture

## Compatibility rule
Rogue gameplay state remains authoritative. The 2.5D renderer is a presentation/client layer.

## Coordinate bridge
A Rogue logical tile `(x, y, elevation)` maps to world space:
`(x * tileScale, elevation * elevationScale, y * tileScale)`.

Movement may be continuous on screen while interaction/collision queries resolve against the logical Rogue map.

## Hub reference
The upstream Rogue Hub layout is 58 x 46 logical cells. Warps, NPCs, background events and scripts are imported as semantic scene data rather than baked into visual geometry.

## Milestones
1. Reproducible Android APK.
2. Native/portable Rogue Core bridge.
3. Save and input bridge.
4. Hub semantic importer.
5. 2.5D Hub renderer.
6. Overworld encounters.
7. Battle presentation modernization.
