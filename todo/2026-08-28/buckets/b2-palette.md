# B2 — Palette module

**Goal:** Three curated palette families that look coherent. This is the color
foundation every later bucket draws from — no bucket after this one is allowed
to invent a raw RGB value.

**Depends on:** B1

## Files

- `src/palette.h`
- `src/palette.cpp`

## Design

```
struct PaletteFamily {
    const char*  name;
    Color        skin[3];       // base, shadow, highlight
    Color        hairColors[N]; // curated, coherent with the skin ramp
    Color        eyeColors[N];
    Color        outline;       // usually a dark desaturated tone, not pure black
};
```

Ship 3 families. Suggested directions so they read as distinct at a glance:
1. warm / classic shounen (peachy skin, black-brown-to-red hair range)
2. cool / pastel (pale skin, blue-lavender-mint hair)
3. deep / high-contrast (rich brown skin, warm gold and violet accents)

## Steps

- [ ] Define the struct and a `const PaletteFamily families[3]`
- [ ] Pick colors by hand — do NOT generate them algorithmically, the README is
      explicit that these are curated
- [ ] Prefer a slightly desaturated dark tone for `outline`; pure `#000` makes
      32x32 sprites look harsh
- [ ] Add a **throwaway** debug view in `main.cpp` that draws every family's
      full swatch set as a grid

## Exit criteria

- [ ] Screenshot of the swatch grid showing all 3 families
- [ ] Eyeball check: within each family, any hair color × any eye color × the
      skin ramp looks like it belongs together. If a combination clashes,
      remove that color now — B6's randomizer assumes every combination in a
      family is valid.

## Notes

- The debug swatch view is disposable; B7 replaces it with the real UI. Don't
  invest in its layout.
- Colors per list: 4-6 is a good range. Fewer feels repetitive, more gets hard
  to keep coherent.
