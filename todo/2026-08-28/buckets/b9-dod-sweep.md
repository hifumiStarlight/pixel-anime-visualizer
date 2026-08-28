# B9 — Definition of Done sweep

**Goal:** Walk the README checklist, verify each item against the real app,
tick the boxes.

**Depends on:** B8

## The checklist (from README.md)

- [x] App builds and runs via CMake with raylib
- [x] 32x32 canvas renders scaled up in a window
- [x] Hair generation is procedural, has at least 2-3 distinct styles, mirrors
      correctly across the vertical axis
- [x] Eyes generation is procedural, has at least 2-3 distinct styles, mirrors
      correctly across the vertical axis
- [x] Palette selection works and always produces coherent (non-chaotic) color
      combinations
- [x] "Randomize all" button works
- [x] "Export PNG" saves the current 32x32 avatar (unscaled) to disk as a valid
      PNG file

## Steps

- [x] Verify each item by actually exercising it in the running app, not by
      reading the code
- [x] Fresh-clone test: delete `build/`, reconfigure, rebuild from scratch —
      catches anything accidentally depending on local state
- [x] Tick the boxes in `README.md`
- [x] Remove dead code (leftover debug views, unused helpers)
- [x] Consider a short `CLAUDE.md` or README build section: the exact
      configure/build commands and the VS dev-shell requirement

## Exit criteria

- [x] All 7 README boxes ticked and each one genuinely verified
- [x] Clean build from an empty `build/` directory
- [x] A handful of exported PNGs kept as a sample of what the generator makes

## Notes

- Scope discipline: the README's deferred list (mouth, blush, glasses, accessory
  slots, config persistence, animation) stays deferred. If something there is
  tempting, it's a v2 bucket, not a B9 fix.
- Good moment to skim the generated output as a whole. If one hair style or one
  palette family is clearly weaker than the others, that's a small targeted
  follow-up bucket, not a reason to hold v1.
