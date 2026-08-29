# v2 roadmap — "feels finished"

v1 shipped: procedural face + hair + eyes + palette, live UI, PNG export, branch
protected. v2 is three small buckets that make the app feel like a finished tool
rather than a tech demo. They compose but don't depend on each other — do them in
any order.

| Bucket | One line | Rough size |
|--------|----------|------------|
| **B10 — Mouth** | A fourth avatar axis, parallel to eyes | ~1 afternoon, mostly pixel tuning |
| **B11 — Config persistence** | Remember the last avatar across launches | Small — validation already exists |
| **B12 — Share string** | Copy/paste a short code for the whole avatar | Small once the wire format is decided |

## Scope discipline (carried over from B9)

Still deferred, still not v2: face shape, blush, glasses / earrings / accessory
slots, expression states, animation, asset-based generation. If B10 makes an
Expression enum tempting, that's a **B13**, not a B10 fix — one axis at a time.

## Why these three

- **B6 made `renderAvatar` pure and colors indices, not `Color`s.** Persistence
  (B11) and share codes (B12) are then just "serialize 7 small integers +
  re-validate", and the validation is already written.
- **B10** is the only one with real iteration cost, and it's the same kind of
  cost as B5: compiling is fast, judging whether a 3-pixel mouth looks like a
  smile or like dirt is not.
