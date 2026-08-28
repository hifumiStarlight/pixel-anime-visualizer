#pragma once

#include "raylib.h"

constexpr int kMaxPaletteColors = 6;
constexpr int kFamilyCount = 3;

struct PaletteFamily {
    const char* name;
    Color skin[3]; // base, shadow, highlight
    int   hairCount;
    Color hairColors[kMaxPaletteColors];
    int   eyeCount;
    Color eyeColors[kMaxPaletteColors];
    Color outline;
};

extern const PaletteFamily families[kFamilyCount];

static_assert(kMaxPaletteColors >= 4 && kMaxPaletteColors <= 8,
              "kMaxPaletteColors should allow 4-8 curated colors per B2");
static_assert(kFamilyCount == 3, "B2 requires exactly 3 families");
