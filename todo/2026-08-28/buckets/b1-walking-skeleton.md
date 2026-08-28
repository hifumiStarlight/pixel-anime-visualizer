# B1 — Walking skeleton

**Goal:** A window opens and shows a hardcoded 32x32 blob, scaled up chunky.
This bucket de-risks the entire toolchain and dependency story before any art
logic exists.

**Depends on:** B0

## Files

- `CMakeLists.txt`
- `src/main.cpp`
- `src/canvas.h`

## Steps

- [x] `CMakeLists.txt`:
      - `cmake_minimum_required(VERSION 3.20)`, C++17, single exe target
      - raylib via `FetchContent` with a **pinned** `GIT_TAG 5.5`
      - `set(BUILD_EXAMPLES OFF)` and `set(BUILD_GAMES OFF)` before
        `FetchContent_MakeAvailable` so we don't compile raylib's samples
- [x] `src/canvas.h`: `struct Canvas { Color px[32*32]; }` plus
      `set(x,y,c)` / `get(x,y)` / `clear(c)` helpers. Plain data, no raylib
      windowing dependency beyond `Color`.
- [x] `src/main.cpp`:
      - `InitWindow`, 60 fps target
      - Fill a Canvas with a hardcoded recognizable blob (a filled circle or
        just a diagonal — anything clearly non-uniform)
      - Upload: `Image` from the Canvas pixel buffer -> `LoadTextureFromImage`
      - `SetTextureFilter(tex, TEXTURE_FILTER_POINT)` — **critical**, without
        this the upscale is blurry and the whole pixel-art premise dies
      - Draw at 16x with `DrawTextureEx` or a dest-rect `DrawTexturePro`

## Exit criteria

- [x] `cmake -B build -G Ninja` configures cleanly
- [x] `cmake --build build` produces an exe
- [x] Window opens showing the blob with hard, visibly square pixels
- [x] Screenshot captured

## Notes

- First configure downloads and compiles raylib from source — expect a few
  minutes. Subsequent configures are cached.
- If the FetchContent download fails, that's a network/proxy issue, not a code
  issue — fall back to a vendored prebuilt raylib rather than fighting it.
- Keep the texture upload in one place; from B2 onward every bucket re-uploads
  the same way whenever the config changes.
