# B10 — Mouth

**Goal:** A fourth avatar axis. 3-4 procedural mouth styles, drawn exactly the
way eyes are: a small parametrized shape at a fixed socket, style alone
determines the pixels, no seed. The one v2 bucket with real pixel-tuning time.

**Depends on:** v1 (B6 config, B7 UI). Independent of B11 / B12.

## Files

- `src/mouth.cpp` (declaration in `generator.h`)
- `src/avatar.h`, `src/avatar.cpp` (config field + validation + randomize)
- `src/ui.cpp` (style cycler)
- `README.md` (move "mouth" out of the deferred list)

## Design

Mirror the eye-socket pattern already in `generator.h`:

```
constexpr int kMouthSocketX = 6;   // left half only; 16..31 mirrored
constexpr int kMouthSocketY = 20;  // below the eye socket (eyes are y 12..16)
constexpr int kMouthSocketW = 4;
constexpr int kMouthSocketH = 3;

enum class MouthStyle { Neutral, Smile, Open, Frown, COUNT };

void drawMouth(Canvas&, MouthStyle, const PaletteFamily&);
```

No `mouthColor` field — the mouth uses a tone already in the family (a darker
skin/lip shade, or the existing outline color). Adding a colour axis for a
3-pixel feature isn't worth the UI space. Signature matches `drawEyes`, **not**
`drawHair`: no seed, fully determined by style.

## Steps

- [x] Add `kMouthSocket*` constants with `static_assert`s bounding them to the
      authored half (x + w <= 16) and the canvas (y + h <= 32), same as the eye
      socket asserts
- [x] `enum class MouthStyle` + `kMouthStyleCount` + a matching
      `static_assert(kMouthStyleCount == N, ...)` and a `mouthStyleName()`
      switch — the same tripwire pattern as `HairStyle` / `EyeStyle`
- [x] Implement `drawMouth`; every write goes through `setPixelMirrored`
- [x] Draw order in `renderAvatar`: face -> hair -> eyes -> **mouth last**, so
      it sits on skin and can never be overdrawn by hair
- [x] `AvatarConfig`: add `MouthStyle mouthStyle`; update
      `isValidAvatarConfig`, `normalizeAvatarConfig`, `randomizeAll`
- [x] `drawControlPanel`: add a mouth-style cycler next to the eye controls,
      wrapping past the last option like the others
- [x] README: add mouth to "Customizable Parameters", remove it from "Out of
      Scope / Explicitly Deferred"

## Exit criteria

- [x] All styles are distinguishable at a glance in the 12-16x preview, not
      just when zoomed
- [x] Mouth never touches the eye socket rows or any hair pixel, for every
      hair style
- [x] Same config -> byte-identical Canvas (regression on the B6 guarantee)
- [x] Randomize All includes the mouth and every combination still reads as a
      coherent face
- [x] Screenshot grid: N mouth styles x a couple of faces

## Notes

- **Center-line hazard, again.** With strict mirroring, a 1px feature authored
  on column 15 becomes 2px wide after the mirror. A "neutral" line and a
  "frown" are easy to get wrong here — author from x 5-6 and check column 15/16
  explicitly, the same failure mode B5 called out for spiky hair.
- A smile that's only 2-3 pixels can read as a smudge. Judge it at real preview
  scale and against a few skin tones before deciding it's done.
- Keep it to one axis. An `Expression` enum that drives eyes + mouth together
  is a real idea but it's **B13**, not part of this bucket.
