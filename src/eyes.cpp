#include "generator.h"

static void pixel(Canvas& c, int x, int y, Color color) {
    setPixelMirrored(c, x, y, color);
}

void drawEyes(Canvas& c, EyeStyle style, Color eyeColor, const PaletteFamily& fam) {
    const int x = kEyeSocketX;
    const int y = kEyeSocketY;
    const Color lash = fam.outline;
    const Color highlight = fam.skin[2];

    switch (style) {
    case EyeStyle::Round:
        for (int i = 0; i < kEyeSocketW; i++) pixel(c, x + i, y, lash);
        pixel(c, x, y + 1, lash);
        pixel(c, x + 3, y + 1, lash);
        pixel(c, x, y + 2, lash);
        pixel(c, x + 3, y + 2, lash);
        pixel(c, x + 1, y + 1, eyeColor);
        pixel(c, x + 2, y + 1, highlight);
        pixel(c, x + 1, y + 2, eyeColor);
        pixel(c, x + 2, y + 2, eyeColor);
        for (int i = 0; i < kEyeSocketW; i++) pixel(c, x + i, y + 3, lash);
        break;

    case EyeStyle::Sharp:
        // The upper lash slopes toward the inner corner of the left eye.
        pixel(c, x, y, lash);
        pixel(c, x + 1, y, lash);
        pixel(c, x + 2, y + 1, lash);
        pixel(c, x + 3, y + 1, lash);
        pixel(c, x, y + 1, lash);
        pixel(c, x, y + 2, lash);
        pixel(c, x + 3, y + 2, lash);
        pixel(c, x + 1, y + 2, eyeColor);
        pixel(c, x + 2, y + 2, highlight);
        pixel(c, x + 1, y + 3, lash);
        pixel(c, x + 2, y + 3, lash);
        pixel(c, x + 3, y + 3, lash);
        break;

    case EyeStyle::Sleepy:
        for (int i = 0; i < kEyeSocketW; i++) {
            pixel(c, x + i, y, lash);
            pixel(c, x + i, y + 1, lash);
        }
        pixel(c, x, y + 2, lash);
        pixel(c, x + 3, y + 2, lash);
        pixel(c, x + 1, y + 2, eyeColor);
        pixel(c, x + 2, y + 2, highlight);
        pixel(c, x, y + 3, lash);
        pixel(c, x + 3, y + 3, lash);
        break;

    case EyeStyle::COUNT:
        break;
    }
}
