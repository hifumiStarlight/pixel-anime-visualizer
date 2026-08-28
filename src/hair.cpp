#include "generator.h"

#include <cstdint>

namespace {

uint32_t nextRandom(uint32_t& state) {
    if (state == 0) state = 0x6D2B79F5u;
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

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
    uint32_t state = seed ^ (0x9E3779B9u * (static_cast<uint32_t>(style) + 1u));
    int fringe = 8;

    // The cap follows the existing head pixels, so it cannot float outside the
    // scalp. Each column gets a bounded, deterministic fringe depth.
    for (int x = 0; x <= 15; x++) {
        int depth = 2 + static_cast<int>(nextRandom(state) % 3u);
        if (style == HairStyle::Spiky) depth += static_cast<int>(nextRandom(state) % 3u);
        else if (style == HairStyle::Bob) depth += static_cast<int>(nextRandom(state) % 2u);
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
        int leftEnd = 18 + static_cast<int>(nextRandom(state) % 4u);
        for (int x = 3; x <= 5; x++) {
            int end = leftEnd + (x == 3 ? 1 : 0);
            for (int y = 9; y <= end && y < 32; y++)
                putHair(canvas, x, y, hairColor, fam, 9, y == 9);
        }
    }

    if (style == HairStyle::Long) {
        int strandEnd = 25 + static_cast<int>(nextRandom(state) % 4u);
        for (int x = 2; x <= 4; x++) {
            int end = strandEnd - (x == 2 ? 1 : 0);
            for (int y = 10; y <= end && y < 32; y++)
                putHair(canvas, x, y, hairColor, fam, 10, y == 10);
        }
    }
}
