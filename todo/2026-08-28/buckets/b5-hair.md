# B5 — Hair

**Goal:** Three procedural hair styles, each with a seeded silhouette that
varies within the style. The biggest and slowest bucket — this is where the
pixel-tuning time goes.

**Depends on:** B4

## Files

- `src/hair.cpp` (declaration in `generator.h`)

## Design

Per the README: hair = *randomized silhouette within an allowed bounding
region* + a *fixed base shading rule*.

```
enum class HairStyle { Spiky, Bob, Long, COUNT };
void drawHair(Canvas&, HairStyle, Color hairColor, uint32_t seed,
              const PaletteFamily&);
```

The seed varies the silhouette **inside** the style's bounding region — it must
never produce hair that floats off the head or clips through the eyes. The
bounding region is the safety rail; randomness only picks within it.

Suggested styles:
1. **Spiky** — randomized spike heights along the top edge, fixed hairline
2. **Bob** — rounded volume with randomized side length and fringe split point
3. **Long** — bob plus randomized side strands running down past the shoulders

## Steps

- [ ] Define a per-style bounding region (min/max row and column extents)
- [ ] Implement silhouette generation: seeded per-column height/extent values,
      clamped to the region
- [ ] Apply the shared shading rule: hair base color, a darker tone near the
      hairline and under overhangs, a lighter tone on the top-facing surface.
      Same light direction as B3.
- [ ] Preserve the hairline over the forehead so the face still reads
- [ ] Guarantee the eye rows are never overdrawn (draw hair before eyes, or
      explicitly exclude the socket rows)
- [ ] All writes through `setPixelMirrored`

## Exit criteria

- [ ] Screenshot grid: 3 styles × 3 seeds = 9 avatars
- [ ] **Every** cell is a plausible avatar — no floating pixels, no hair
      covering the eyes, no gap between hair and scalp
- [ ] The 3 styles are distinguishable by silhouette alone
- [ ] Same seed + same style always produces identical pixels

## Notes

- If a seed can produce a broken silhouette, tighten the bounding region rather
  than special-casing that seed. The region is the correctness mechanism.
- Center column: with strict mirroring, column 15/16 is the part line. Spiky
  styles can look accidentally symmetrical-weird there — check it explicitly.
- Budget real iteration time. Compiling is fast; judging whether hair looks
  good is not.
