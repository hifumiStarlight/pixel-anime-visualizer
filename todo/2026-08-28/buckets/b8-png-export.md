# B8 — PNG export

**Goal:** Save the current avatar as a real 32x32 PNG. Auto-named, zero
friction, so rapid rerolling stays fast.

**Depends on:** B7

## Files

- `src/export.cpp`
- `src/ui.cpp` (add the button)

## Design

Writes `./exports/avatar_001.png`, `avatar_002.png`, … next to the exe. The
counter is found by scanning the folder for the highest existing number and
adding one, so it survives restarts without any saved state.

## Steps

- [ ] Create `./exports/` if it doesn't exist
- [ ] Scan for existing `avatar_*.png` and pick the next free index
- [ ] Export the **raw 32x32 Canvas**, not the scaled-up preview texture. Build
      a fresh `Image` from the config's Canvas buffer and call
      `ExportImage`. Never read back the on-screen render target.
- [ ] Add the Export PNG button to the panel
- [ ] Flash the saved path on screen for a couple of seconds as confirmation
- [ ] Handle failure (read-only folder, disk full) with a visible message
      rather than silently doing nothing

## Exit criteria

- [ ] Button writes a file
- [ ] File opens in an image viewer and is **exactly 32x32**
- [ ] Exported pixels match the preview exactly
- [ ] Exporting twice produces two files, no overwrite
- [ ] Alpha is correct — background transparent (or intentionally opaque, but
      decide, don't leave it accidental)

## Notes

- The "export the scaled texture by mistake" bug is the single most likely
  error in this bucket, and it produces a file that looks fine until you check
  its dimensions. Verify the size explicitly.
- Because B6 made `renderAvatar` a pure function, export and preview share one
  source of truth. Keep it that way.
