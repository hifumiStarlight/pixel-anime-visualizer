# B0 — Toolchain on PATH

**Goal:** Get a working command-line C++ build environment. No project code yet.

## Context

Machine already has:
- Visual Studio 2022 Community with MSVC toolset 14.40.33807 (compiler exists,
  but `cl` is not on PATH — it lives inside the VS install)
- Git
- winget

Missing from PATH: `cmake`, `ninja`, `cl`.

VS ships a bundled CMake 3.28.3 at
`C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`
but we decided to install standalone tools instead for a cleaner CLI loop.

## Steps

- [ ] `winget install Kitware.CMake`
- [ ] `winget install Ninja-build.Ninja`
- [ ] Open a **fresh** shell (winget PATH changes don't apply to the current one)
- [ ] Confirm MSVC is reachable — either via the "x64 Native Tools Command
      Prompt for VS 2022", or by sourcing `VsDevCmd.bat -arch=x64` in the shell
      used for builds
- [ ] Add `.gitignore` at repo root:
      ```
      build/
      exports/
      ```

## Exit criteria

- [ ] `cmake --version` works in a fresh shell
- [ ] `ninja --version` works in a fresh shell
- [ ] `cl` resolves inside the VS dev shell
- [ ] `.gitignore` committed

## Notes

- Ninja is optional in principle (the VS generator works too) but gives much
  faster incremental builds, which matters because B5 is a tight
  edit-compile-look loop.
- If winget's CMake lands but PATH still looks stale, the installer put it in
  `C:\Program Files\CMake\bin` — check there before debugging further.
