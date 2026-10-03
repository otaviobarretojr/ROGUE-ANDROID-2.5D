# Rogue Bridge Contract

The bridge is deliberately renderer-agnostic.

## Input
Android touch/gamepad input is normalized into a GBA-compatible bit mask. The Rogue Core consumes the mask; the 2.5D presentation may also use analog input later without changing gameplay actions.

## Storage
Android provides an app-private writable directory. The bridge exposes the path to the portable core. ROM data, saves and keys are never committed.

## World state
The minimum presentation state is:
- map id
- logical x/y
- elevation
- facing direction

Later revisions add object events, warps, background events, encounter actors and dynamic Hub upgrade state.

## Authority
The Rogue Core owns gameplay truth. The renderer must not independently resolve battles, progression, inventory, RNG or scripts.
