#include "generator.h"
#include "rng.h"

#include <cstdint>

namespace {

Color scaleColor(Color color, int percent) {
    color.r = static_cast<unsigned char>((static_cast<int>(color.r) * percent) / 100);
    color.g = static_cast<unsigned char>((static_cast<int>(color.g) * percent) / 100);
    color.b = static_cast<unsigned char>((static_cast<int>(color.b) * percent) / 100);
    return color;
}

bool socketPixel(int x, int y) {
    return y >= kEyeSocketY && y < kEyeSocketY + kEyeSocketH &&
           x >= kEyeSocketX && x < kEyeSocketX + kEyeSocketW;
}

void putHair(Canvas& canvas, int x, int y, Color base, const PaletteFamily& fam,
             int fringeRow, bool topFacing) {
    (void)fam;
    if (x < 0 || x > 15 || y < 0 || y >= 32 || socketPixel(x, y)) return;

    Color color = base;
    if (y == fringeRow || y == fringeRow + 1) color = scaleColor(base, 72);
    else if (topFacing) color = scaleColor(base, 118);
    // The helper is the only write path, preserving the authored-half contract.
    setPixelMirrored(canvas, x, y, color);
}

} // namespace

void drawHair(Canvas& canvas, HairStyle style, Color hairColor, uint32_t seed,
              const PaletteFamily& fam) {
    (void)fam;
    uint32_t state = seed ^ (0x9E3779B9u * (static_cast<uint32_t>(style) + 1u));
    int fringe = 8;

    // The cap follows the existing head pixels, so it cannot float outside the
    // scalp. Each column gets a bounded, deterministic fringe depth.
    for (int x = 0; x <= 15; x++) {
        int depth = 2 + avatar_rng::bounded(state, 3);
        if (style == HairStyle::Spiky) depth += avatar_rng::bounded(state, 3);
        else if (style == HairStyle::Bob) depth += avatar_rng::bounded(state, 2);
        int end = fringe + depth;
        if (style == HairStyle::Spiky) end = fringe - 1 + (x == 15 ? 1 : depth / 2);
        if (style == HairStyle::Bob) end = fringe + (x < 6 ? 1 : 0);
        if (end > 11) end = 11;

        for (int y = 0; y <= end; y++) {
            if (canvas.get(x, y).a != 0)
                putHair(canvas, x, y, hairColor, fam, end, y == 0 || y < end - 2);
        }
    }

    if (style == HairStyle::Bob || style == HairStyle::Long) {
        int leftEnd = 18 + avatar_rng::bounded(state, 4);
        for (int x = 3; x <= 5; x++) {
            int end = leftEnd + (x == 3 ? 1 : 0);
            for (int y = 9; y <= end && y < 32; y++)
                putHair(canvas, x, y, hairColor, fam, 9, y == 9);
        }
    }

    if (style == HairStyle::Long) {
        int strandEnd = 25 + avatar_rng::bounded(state, 4);
        for (int x = 2; x <= 4; x++) {
            int end = strandEnd - (x == 2 ? 1 : 0);
            for (int y = 10; y <= end && y < 32; y++)
                putHair(canvas, x, y, hairColor, fam, 10, y == 10);
        }
    }
}
