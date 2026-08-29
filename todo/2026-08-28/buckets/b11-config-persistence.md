# B11 — Config persistence

**Goal:** The app remembers the last avatar. Load it on startup, save it when it
changes. One of the README's explicitly-deferred v1 ideas, now in scope.

**Depends on:** v1 (B6 config). Independent of B10 / B12.

## Files

- `src/config_io.h`, `src/config_io.cpp` (new)
- `src/main.cpp` (load on startup, save on change/exit)
- `.gitignore` (ignore the saved file)

## Design

Write `avatar.ini` next to the exe: plain `key=value`, one field per line.

```
version=1
paletteFamily=2
hairStyle=0
hairColor=1
eyeStyle=2
eyeColor=0
seed=2847713991
```

Plain text, no JSON dependency, diffable, hand-editable. Enums are written as
their `int` value. **The file is never trusted:** every load runs through the
B6 validation before the config is used.

```
void saveConfig(const char* path, const AvatarConfig&);
bool loadConfig(const char* path, AvatarConfig& out);  // false = missing/unusable
```

## Steps

- [x] `saveConfig`: write every `AvatarConfig` field as an int, plus a
      `version` line
- [x] `loadConfig`: parse `key=value` lines; a missing file is **not** an error,
      just return false; unknown keys ignored; missing keys -> caller's default
- [x] After a successful parse, run `normalizeAvatarConfig` +
      `isValidAvatarConfig`. Out-of-range indices, a stale `hairColor` for the
      wrong family, garbage values -> fall back to default or `randomizeAll`,
      never crash, never render off-palette
- [x] `main.cpp` startup: `loadConfig` -> if false, pick a default (fixed
      config or one `randomizeAll`)
- [x] `main.cpp` save trigger: **on exit** is the simplest and enough. If you
      want save-on-change, gate it behind the existing "config actually
      changed" dirty check from B7 so you're not writing every frame
- [x] Path: use `GetApplicationDirectory()` so the file lands next to the exe
      like `exports/`, not wherever the shell's cwd happens to be
- [x] Add `avatar.ini` to `.gitignore`

## Exit criteria

- [x] Change the avatar, close, reopen -> the same avatar is on screen
- [x] Delete `avatar.ini` -> app starts cleanly with a default
- [x] Corrupt `avatar.ini` (garbage text, out-of-range indices, half the keys
      missing) -> app starts cleanly, no crash, no off-palette render
- [x] The file is human-readable and editing a value by hand changes the
      loaded avatar

## Notes

- This is where "colors are indices, not `Color`s" (B6) pays off: persistence
  is 6 ints + a seed, and the validation you'd need is already written.
- Never serialize a raylib `Color` or the `Canvas` buffer. `AvatarConfig` is
  the source of truth; pixels are derived.
- `version` line costs nothing now and means B10 (adds `mouthStyle`) or B12 can
  bump it and detect old files instead of misreading them.
