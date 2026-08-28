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

extern const PaletteFamily families[];
