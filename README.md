# Pixel Anime Avatar Generator

A small C++ desktop app that procedurally generates 32x32 pixel-art anime-style
avatars, with a live customizer UI for hair, eyes, and palette. Built for fun /
vibe coding — not a serious engineering project, optimize for "looks cool fast"
over architectural purity.

## Goals

- Procedurally generate a chibi/anime-style pixel avatar face from rules, not
  premade art assets.
- Let the user tweak hair style, eye style, and palette live via UI controls
  and immediately see the result.
- Export the current avatar as a PNG.

## Non-goals (v1)

- No mouth, face shape, or accessory customization yet (hair + eyes + palette
  only).
- No animation, no full-body sprites, no saving/loading a config file — only
  PNG export.
- No networking, no asset packs, no sprite sheets from external artists.

## Tech Stack

- **Language:** C++17
- **Graphics/windowing:** raylib
- **Build:** CMake (single executable target)
- **Output format:** 32x32 pixel canvas, rendered scaled-up (e.g. 8x-16x) in
  the window so pixels are chunky and visible; exported PNG is the raw 32x32
  (not the scaled-up version).

## Build

Use a Visual Studio 2022 x64 developer shell, then run:

```text
cmake -B build -G Ninja
cmake --build build
```

Run `build/pixel_anime_visualizer.exe`. PNG exports are written to `exports/`.

## Sprite Spec

- **Canvas size:** 32x32 pixels.
- **Symmetry:** Strict left-right mirror. Only the left half (columns 0-15) is
  ever computed by generation rules; column 16-31 is a mirrored copy. This
  keeps every generated face structurally valid by construction.
- **Generation approach:** Fully procedural / rule-based pixel placement — no
  premade sprite/asset library. Hair and eyes are drawn by small deterministic
  or randomized algorithms (e.g. hair = randomized silhouette within allowed
  bounding region + fixed base shading rule; eyes = a small parametrized
  shape drawn at a fixed eye-socket position).
- **Palette:** Fixed, curated anime-style palette(s) baked into the program
  (skin tones, hair colors, eye colors chosen to look coherent together —
  not arbitrary RGB). A "randomize" action picks a valid combination from
  this palette, it never generates arbitrary/chaotic RGB values.

## Customizable Parameters (v1 scope)

1. **Hair** — style/silhouette (a small enumerated set of procedural hair
   shapes) + hair color (from the curated palette).
2. **Eyes** — style/shape (a small enumerated set of procedural eye shapes)
   + eye color (from the curated palette).
3. **Palette** — overall color scheme selection (skin tone + which curated
   palette family hair/eye colors are drawn from).

Everything else (face shape, mouth, accessories) is explicitly out of scope
for v1 and can be added later.

## Interaction Loop

- App opens with a live preview window showing the current avatar, scaled up
  large enough to see individual pixels clearly.
- UI controls (buttons/sliders — exact widget choice left to implementation,
  raylib immediate-mode GUI or similar) let the user:
  - Cycle/select hair style and hair color
  - Cycle/select eye style and eye color
  - Cycle/select palette family
  - Trigger "randomize all" for a full reroll
  - Trigger "export PNG" to save the current avatar to disk
- Every change updates the preview immediately (no separate "apply" step).

## Out of Scope / Explicitly Deferred Ideas (for later, not v1)

- Mouth, face shape, blush, glasses, earrings, other accessories
- Remembering/reloading last config on relaunch
- Full color randomization outside the curated palette
- Any animation or multiple expression states
- Any asset-based (non-procedural) generation

## Definition of Done (v1)

- [x] App builds and runs via CMake with raylib
- [x] 32x32 canvas renders scaled up in a window
- [x] Hair generation is procedural, has at least 2-3 distinct styles, mirrors
      correctly across the vertical axis
- [x] Eyes generation is procedural, has at least 2-3 distinct styles, mirrors
      correctly across the vertical axis
- [x] Palette selection works and always produces coherent (non-chaotic)
      color combinations
- [x] "Randomize all" button works
- [x] "Export PNG" saves the current 32x32 avatar (unscaled) to disk as a
      valid PNG file
