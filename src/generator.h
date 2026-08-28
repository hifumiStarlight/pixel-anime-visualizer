#pragma once

#include "canvas.h"
#include "palette.h"
#include <cstdint>

// Fixed eye socket anchor (left eye; right is mirrored). Every eye style draws
// inside this box, so B4 can rely on a stable position.
constexpr int kEyeSocketX = 6;
constexpr int kEyeSocketY = 12;
constexpr int kEyeSocketW = 4;
constexpr int kEyeSocketH = 4;

static_assert(kEyeSocketX >= 0 && kEyeSocketX + kEyeSocketW <= 16,
              "eye socket must stay in authored half 0..15 (mirrored to 16..31)");
static_assert(kEyeSocketY >= 0 && kEyeSocketY + kEyeSocketH <= 32,
              "eye socket must fit vertically in 32px canvas");

enum class EyeStyle { Round, Sharp, Sleepy, COUNT };

enum class HairStyle { Spiky, Bob, Long, COUNT };

constexpr int kHairStyleCount = static_cast<int>(HairStyle::COUNT);
static_assert(kHairStyleCount == 3, "B5 requires exactly 3 hair styles");

// Usable count excluding sentinel - prefer this over raw COUNT in loops.
constexpr int kEyeStyleCount = static_cast<int>(EyeStyle::COUNT);
static_assert(kEyeStyleCount == 3, "labels and switch in eyes.cpp assume 3 styles");

inline const char* hairStyleName(HairStyle s) {
    switch (s) {
    case HairStyle::Spiky: return "spiky";
    case HairStyle::Bob:   return "bob";
    case HairStyle::Long:  return "long";
    default:               return "unknown";
    }
}

inline const char* eyeStyleName(EyeStyle s) {
    switch (s) {
    case EyeStyle::Round:  return "round";
    case EyeStyle::Sharp:  return "sharp";
    case EyeStyle::Sleepy: return "sleepy";
    default:               return "unknown";
    }
}

// Only columns 0-15 are authored; 16-31 are a mirrored copy. Every sprite
// drawing routine writes through this single chokepoint.
void setPixelMirrored(Canvas& c, int x, int y, Color col);

// Head silhouette with skin shading + outline (no hair/eyes yet).
void drawFace(Canvas& c, const PaletteFamily& fam);

void drawHair(Canvas& c, HairStyle style, Color hairColor, uint32_t seed,
              const PaletteFamily& fam);

void drawEyes(Canvas& c, EyeStyle style, Color eyeColor, const PaletteFamily& fam);
