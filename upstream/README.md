# Upstream Rogue source

The Android project does not vendor a ROM.

For core-port development we reference:
- repository: MinZe25/pokeemerald-rogue-vita
- branch: vita_port

The source is fetched into `upstream/pokeemerald-rogue` by `scripts/fetch_rogue_core.sh`.
That directory is ignored by Git so Android-specific work remains isolated.

Important: the existing portable Makefile targets SDL2/Linux/Windows/Vita and assumes 32-bit host flags outside Vita. Android therefore gets its own NDK build adapter rather than invoking that Makefile unchanged.
