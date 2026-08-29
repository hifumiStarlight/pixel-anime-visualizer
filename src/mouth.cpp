#include "generator.h"
#include <cassert>

static void pixel(Canvas& c, int x, int y, Color color) {
    setPixelMirrored(c, x, y, color);
}

void drawMouth(Canvas& c, MouthStyle style, const PaletteFamily& fam) {
    const int x = kMouthSocketX;
    const int y = kMouthSocketY;
    // The lower face (y >= 15) is shaded with fam.skin[1], so the outline is
    // the only family tone dark enough to read as a mouth on every palette.
    const Color mouth = fam.outline;

    switch (style) {
    case MouthStyle::Neutral:
        // A straight line; authored through x+4 so the mirror produces a single
        // unbroken 8px dash, not two stubs with a gap at the center seam.
        for (int i = 1; i <= 4; i++) pixel(c, x + i, y + 1, mouth);
        break;

    case MouthStyle::Smile:
        // U: corners raised one row. The corner column is authored as x+1 only,
        // never x+4, so the mirrored seam (cols 15/16) stays open and the curve
        // reads as one smooth arc instead of a zigzag.
        pixel(c, x + 1, y, mouth);
        for (int i = 2; i <= 4; i++) pixel(c, x + i, y + 1, mouth);
        break;

    case MouthStyle::Open:
        // Rounded block three rows tall; visibly bigger than the 1px lines so
        // it reads as an open mouth. Each row narrows toward the seam.
        for (int i = 1; i <= 4; i++) pixel(c, x + i, y, mouth);
        for (int i = 2; i <= 4; i++) pixel(c, x + i, y + 1, mouth);
        pixel(c, x + 3, y + 2, mouth);
        pixel(c, x + 4, y + 2, mouth);
        break;

    case MouthStyle::Frown:
        // Inverted U, the vertical mirror of Smile: corners droop one row.
        for (int i = 2; i <= 4; i++) pixel(c, x + i, y, mouth);
        pixel(c, x + 1, y + 1, mouth);
        break;

    case MouthStyle::COUNT:
        assert(false && "MouthStyle::COUNT is a sentinel, not a drawable style");
        break;
    default:
        assert(false && "unhandled MouthStyle");
        break;
    }
}