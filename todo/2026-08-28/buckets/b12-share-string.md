# B12 — Share string

**Goal:** Encode the whole avatar as a short, copy-pasteable code. Paste one
back to load that exact avatar. Turns "I rolled a cool one" into something you
can drop in a chat message without the PNG.

**Depends on:** v1 (B6 config). Best done *after* B10 so `mouthStyle` is in the
format from the start. Independent of B11.

## Files

- `src/avatar.h`, `src/avatar.cpp` (`encodeConfig` / `decodeConfig`)
- `src/ui.cpp` (Copy code button, Load code button)

## Design

`AvatarConfig` is a handful of small integers plus a `uint32_t seed`. The job:
pack them, turn the bytes into text from a safe alphabet, prefix a version tag
so a later format change is detectable. `decodeConfig` reverses it and then runs
`normalizeAvatarConfig` — a decoded config is treated exactly as untrusted as a
loaded file in B11.

```
std::string encodeConfig(const AvatarConfig&);
bool        decodeConfig(std::string_view code, AvatarConfig& out);
```

The UI reuses the existing status-line mechanism (`status`, `statusSeconds`)
for "Copied!" / "Invalid code" feedback. `SetClipboardText` /
`GetClipboardText` (raylib) avoid needing a real text-input widget.

## Wire format

<!-- TODO(human): specify the exact wire format for the share string here,
     2-10 lines. This is the one real design decision in the bucket; the steps
     below are deliberately written against whatever you pick. Cover:
       - what fields are packed and in what order
       - the text alphabet (hex / base32 Crockford / base36 / ...)
       - version signalling (leading char? byte?)
       - checksum or not (and which)
       - how long the resulting code ends up (rough char count)
-->

## Steps

- [ ] Implement `encodeConfig` to the format above: version tag + packed fields
      (+ checksum if chosen)
- [ ] Implement `decodeConfig`: reject wrong version, wrong length, bad
      characters, bad checksum — each with `return false`, never a throw or a
      partial write to `out`
- [ ] On a successful parse, run `normalizeAvatarConfig` before handing the
      config back, same guarantee as B11
- [ ] UI: "Copy code" button -> `encodeConfig` -> `SetClipboardText`, flash
      "Copied"
- [ ] UI: "Load code" button -> `GetClipboardText` -> `decodeConfig`; on success
      swap the config and re-render, on failure flash "Invalid code" and change
      nothing
- [ ] Round-trip test: for many random configs, `decodeConfig(encodeConfig(c))`
      yields a config equal to `c`

## Exit criteria

- [ ] Encode a config, decode the string -> identical config
- [ ] The same config always encodes to the same string
- [ ] A truncated, hand-edited, or foreign string is rejected with a visible
      message — never a crash, never a garbage avatar
- [ ] The code is short enough to paste inline in chat (aim under ~16 chars)
- [ ] Version tag is present and a wrong tag is rejected cleanly

## Notes

- **Don't `memcpy` the struct.** Padding, enum width, and endianness are
  implementation-defined; a raw dump breaks across compilers — exactly what a
  share code needs to survive. Pack field by field.
- `seed` is a full `uint32_t` and will dominate the length. If codes come out
  too long, narrow `seed` to 16 bits *at the `AvatarConfig` level* (65k
  silhouettes is still plenty) rather than truncating inside the encoder.
- Keep `encodeConfig` / `decodeConfig` free of raylib and UI — same discipline
  that keeps `renderAvatar` pure and B8's export trivially correct.
