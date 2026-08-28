#pragma once

#include "canvas.h"
#include "palette.h"

// Fixed eye socket anchor (left eye; right is mirrored). Every eye style draws
// inside this box, so B4 can rely on a stable position.
constexpr int kEyeSocketX = 6;
constexpr int kEyeSocketY = 12;
constexpr int kEyeSocketW = 4;
constexpr int kEyeSocketH = 4;

enum class EyeStyle { Round, Sharp, Sleepy, COUNT };

// Only columns 0-15 are authored; 16-31 are a mirrored copy. Every sprite
// drawing routine writes through this single chokepoint.
void setPixelMirrored(Canvas& c, int x, int y, Color col);

// Head silhouette with skin shading + outline (no hair/eyes yet).
void drawFace(Canvas& c, const PaletteFamily& fam);

void drawEyes(Canvas& c, EyeStyle style, Color eyeColor, const PaletteFamily& fam);