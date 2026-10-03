# Rogue Android 2.5D

Experimental Android-native modernization layer for Pokémon Emerald Rogue.

## Goals
- Preserve Rogue gameplay/rules while modernizing presentation.
- Android arm64-v8a first.
- 60 FPS target.
- Touch + gamepad input.
- 2.5D overworld renderer with a compatibility bridge to Rogue map/event data.
- Never commit ROMs, extracted commercial assets, signing keys, or saves.

## Status
Foundation / Prototype 0.1.

The first milestone intentionally boots a minimal Android surface. Rogue Core integration follows only after CI and APK packaging are reproducible.

## Architecture
```
Rogue Core
   |
Rogue Bridge (map/events/input/save)
   |
2.5D Runtime
   |
Android Platform (Kotlin/NDK)
```

## Legal / repository hygiene
This repository contains no game ROM. Local ROM import/validation will be implemented separately.
