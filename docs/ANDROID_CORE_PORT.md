# Android Rogue Core port

## Upstream frame contract
The portable port initializes gameplay with `AgbMain()`. Each game tick calls:
1. `MainLoop()`
2. `RunDMAsAndVBlank()`
3. rendering/audio are platform concerns

`MainLoop()` reads keys through `Platform_GetKeyInput()`.

## Android strategy
We do not invoke the SDL2/Vita Makefile unchanged. Android is arm64 and uses the NDK/Clang toolchain.

The migration is split into:
1. host-side upstream code/data generation;
2. NDK compilation probes;
3. game-side relocatable object;
4. Android platform implementations for input/save/audio/time;
5. ROM-assets loader using a ROM selected by the user;
6. link into `librogue25d.so`.

## Asset policy
No game ROM is stored in Git or bundled in CI. ROM-assets mode is retained so compatible assets can be loaded locally from a user-selected ROM at runtime.

## First probe
`scripts/android_core_probe.sh` compiles `src/platform/system.c` and `src/main.c` for aarch64. This intentionally starts small: every failure becomes a concrete portability task rather than hiding hundreds of errors in a monolithic build.
