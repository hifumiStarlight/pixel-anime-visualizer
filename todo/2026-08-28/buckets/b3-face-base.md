# B3 — Face base + the mirror primitive

**Goal:** A bald head renders, correctly mirrored. Establishes the symmetry
invariant once, so no later bucket has to think about it.

**Depends on:** B2

## Files

- `src/generator.h`
- `src/generator.cpp`

## The invariant

Per the README: only columns 0-15 are ever computed. Columns 16-31 are a
mirrored copy. Every drawing routine from here on writes through:

```
void setPixelMirrored(Canvas& c, int x, int y, Color col);
// asserts x is in [0,15]; writes both (x,y) and (31-x, y)
```

Nothing after this bucket calls `Canvas::set` directly for sprite content. That
single chokepoint is what makes every generated face structurally valid by
construction.

## Steps

- [ ] Implement `setPixelMirrored` with a debug assert on `x <= 15`
- [ ] Draw the head silhouette: a chibi-proportioned rounded shape. Head should
      dominate the canvas — roughly the top 2/3 — with a small neck and
      shoulder hint at the bottom.
- [ ] Apply skin shading using the family's 3-tone ramp (base, shadow along one
      edge, subtle highlight)
- [ ] Outline the silhouette with `family.outline`
- [ ] Leave the eye socket rows clearly identifiable — B4 needs a fixed,
      documented anchor position. Write the chosen socket coordinates into a
      named constant, not a magic number at the call site.

## Exit criteria

- [ ] Bald head renders on screen
- [ ] Symmetry verified: for every row, pixel `(x,y) == (31-x,y)`
- [ ] Switching palette family visibly changes the skin tone
- [ ] Screenshot of the same head across all 3 families

## Notes

- Shading direction: pick one light source (top-left is conventional) and keep
  it consistent across B3/B4/B5, or the sprite reads as muddy.
- A 32x32 chibi face is mostly head. Resist making the neck/shoulders large —
  they steal rows the hair silhouette needs.
