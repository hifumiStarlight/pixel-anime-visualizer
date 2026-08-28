# B6 — Config + deterministic RNG

**Goal:** One struct holds the whole avatar. Same config always produces the
same pixels. "Randomize all" produces a valid, coherent combination.

**Depends on:** B5

## Files

- `src/avatar.h`
- `src/avatar.cpp`
- `src/rng.h`

## Design

```
struct AvatarConfig {
    int       paletteFamily;  // index into families[]
    HairStyle hairStyle;
    int       hairColor;      // index into family.hairColors
    EyeStyle  eyeStyle;
    int       eyeColor;       // index into family.eyeColors
    uint32_t  seed;           // silhouette variation, independent of style
};

Canvas renderAvatar(const AvatarConfig&);
AvatarConfig randomizeAll(uint32_t& rngState);
```

Color fields are **indices, not Colors**. That keeps every config valid by
construction: switching palette family can never leave a hair color that
doesn't belong to it.

## Steps

- [x] Define `AvatarConfig`
- [x] `src/rng.h`: small splitmix64 or xorshift32. Do **not** use
      `rand()` — its sequence isn't reproducible across runs/platforms, which
      breaks the determinism guarantee.
- [x] `renderAvatar`: pure function, config in, Canvas out. Calls face → hair →
      eyes in the order B5 established.
- [x] `randomizeAll`: pick family first, then pick color indices *within* that
      family, then styles, then a fresh seed
- [x] Clamp color indices when the family changes (a family may have fewer
      colors than the previously selected index)

## Exit criteria

- [x] Rendering the same config twice yields byte-identical Canvas buffers
- [x] Randomize 20 times: every result is coherent, none use off-palette colors
- [x] Changing only `seed` changes the hair silhouette but nothing else
- [x] Changing only `hairStyle` keeps the same seed's "feel" within the region

## Notes

- Keep `renderAvatar` free of raylib windowing calls. It's a pure
  config → pixels function; that's what makes B8's export trivially correct
  (export renders the same buffer the preview shows).
- This is the bucket that makes the app feel like a generator rather than three
  independent toggles.
