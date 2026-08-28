# B7 — raygui control panel

**Goal:** Real UI controls for all five axes plus Randomize All. Every change
updates the preview immediately, with no apply step.

**Depends on:** B6

## Files

- `CMakeLists.txt` (add raygui)
- `src/ui.cpp`
- `src/main.cpp` (wire up)

## Steps

- [ ] Add raygui via `FetchContent` (header-only; pin a tag compatible with
      raylib 5.5). Define `RAYGUI_IMPLEMENTATION` in **exactly one** .cpp file
      — doing it in two is the classic link-error trap.
- [ ] Widen the window: preview on the left, control panel on the right
- [ ] Controls:
      - Palette family — cycle or dropdown
      - Hair style — cycle
      - Hair color — cycle (or a row of swatch buttons, nicer)
      - Eye style — cycle
      - Eye color — cycle
      - **Randomize All** button
      - (Export button lands in B8 — leave space for it)
- [ ] Re-render the Canvas and re-upload the texture **only when the config
      actually changed**, not every frame
- [ ] Show the current seed as text somewhere — useful when a good avatar
      appears and you want to keep it
- [ ] Delete the B2 debug swatch view

## Exit criteria

- [ ] Every control changes the preview instantly
- [ ] Randomize All rerolls everything at once
- [ ] Cycling past the last option wraps to the first
- [ ] Switching palette family doesn't leave an invalid color selected
- [ ] Screenshot of the full app window

## Notes

- Dropdowns in raygui capture input while open — if you use them, handle the
  "menu is open" state or clicks will fall through to whatever is behind.
- Preview scale: pick a factor that leaves the panel room. 12x-16x on a 32x32
  canvas is 384-512 px of preview, which is plenty.
- This is the first bucket where the app is genuinely *usable*. Spend a minute
  actually playing with it — that's the fastest way to find bad palette
  combinations or a hair style that's weaker than the other two.
