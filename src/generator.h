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

// Fixed mouth socket anchor, centered on the chin (left half; right is
// mirrored). The face narrows at the bottom, so the mouth sits at the center
// columns where the chin is widest (y 18..20, x 11..15).
constexpr int kMouthSocketX = 11;
constexpr int kMouthSocketY = 18;
constexpr int kMouthSocketW = 5;
constexpr int kMouthSocketH = 3;

static_assert(kMouthSocketX >= 0 && kMouthSocketX + kMouthSocketW <= 16,
              "mouth socket must stay in authored half 0..15 (mirrored to 16..31)");
static_assert(kMouthSocketY >= 0 && kMouthSocketY + kMouthSocketH <= 32,
              "mouth socket must fit vertically in 32px canvas");

enum class MouthStyle { Neutral, Smile, Open, Frown, COUNT };

constexpr int kHairStyleCount = static_cast<int>(HairStyle::COUNT);
static_assert(kHairStyleCount == 3, "B5 requires exactly 3 hair styles");

// Usable count excluding sentinel - prefer this over raw COUNT in loops.
constexpr int kEyeStyleCount = static_cast<int>(EyeStyle::COUNT);
static_assert(kEyeStyleCount == 3, "labels and switch in eyes.cpp assume 3 styles");

constexpr int kMouthStyleCount = static_cast<int>(MouthStyle::COUNT);
static_assert(kMouthStyleCount == 4, "labels and switch in mouth.cpp assume 4 styles");

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

inline const char* mouthStyleName(MouthStyle s) {
    switch (s) {
    case MouthStyle::Neutral: return "neutral";
    case MouthStyle::Smile:   return "smile";
    case MouthStyle::Open:    return "open";
    case MouthStyle::Frown:   return "frown";
    default:                  return "unknown";
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

void drawMouth(Canvas& c, MouthStyle style, const PaletteFamily& fam);
