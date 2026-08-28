# B4 — Eyes

**Goal:** Three procedural eye styles, drawn at the fixed socket anchor from
B3, mirrored correctly.

**Depends on:** B3

## Files

- `src/eyes.cpp` (declaration in `generator.h`)

## Design

```
enum class EyeStyle { Round, Sharp, Sleepy, COUNT };
void drawEyes(Canvas&, EyeStyle, Color eyeColor, const PaletteFamily&);
```

Because of mirroring, you only ever author the **left** eye. The right one is
free.

Suggested styles (aim for silhouette contrast, not detail):
1. **Round** — tall, large iris, big highlight pixel. Reads young/friendly.
2. **Sharp** — narrower, angled top lash line, smaller iris. Reads cool/serious.
3. **Sleepy** — short, heavy upper lid, iris partly covered. Reads calm/bored.

Each eye is roughly 3-4 px wide and 3-5 px tall at this canvas size.

## Steps

- [ ] Implement the three styles as small parametrized routines
- [ ] Every eye gets: lash/outline pixels, iris in `eyeColor`, and **one**
      highlight pixel. The highlight is what makes it read as anime rather than
      as a dot.
- [ ] All writes go through `setPixelMirrored`
- [ ] Only the eye color varies with user choice; lash and highlight come from
      the palette family

## Exit criteria

- [ ] Screenshot showing all 3 styles on the same head
- [ ] The three are distinguishable at 1x, not just zoomed in — if they only
      differ when magnified, push the silhouettes further apart
- [ ] Symmetry still holds
- [ ] Eye color choice visibly changes the iris

## Notes

- At 32x32 a single pixel is a large feature. Changing one lash pixel changes
  the whole expression — expect to iterate here.
- Eye spacing matters more than eye shape for "does this look like a face".
  If something feels off, adjust the gap between the sockets before redrawing
  the eyes themselves.
