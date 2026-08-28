#include "generator.h"
#include <cassert>

static bool inEllipse(float px, float py, float cx, float cy, float rx, float ry) {
    float dx = (px - cx) / rx;
    float dy = (py - cy) / ry;
    return dx * dx + dy * dy <= 1.0f;
}

void setPixelMirrored(Canvas& c, int x, int y, Color col) {
    assert(x >= 0 && x <= 15 && y >= 0 && y < 32);
    if (x < 0 || x > 15 || y < 0 || y >= 32) return;
    c.set(x, y, col);
    c.set(31 - x, y, col);
}

static bool isFilled(const Canvas& c, int x, int y) {
    if (x < 0 || x >= 32 || y < 0 || y >= 32) return false;
    return c.get(x, y).a != 0;
}

void drawFace(Canvas& c, const PaletteFamily& fam) {
    c.clear({ 0, 0, 0, 0 });

    // Head dominates the canvas; small neck + shoulder hint at the bottom.
    bool inside[16][32];
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x <= 15; x++) {
            bool inHead = inEllipse((float)x, (float)y, 15.5f, 10.5f, 12.0f, 10.5f);
            bool inNeck = (x >= 13 && x <= 15) && (y >= 21 && y <= 23);
            bool inShoulder =
                ((y >= 24 && y <= 25) && x >= 8) ||
                ((y >= 26 && y <= 28) && x >= 6);
            inside[x][y] = inHead || inNeck || inShoulder;
        }
    }

    // Top-down shading: highlight on the crown, shadow under the face, subtle
    // rim occlusion on the outer columns.
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x <= 15; x++) {
            if (!inside[x][y]) continue;

            Color col;
            if (y >= 21) {
                col = fam.skin[1]; // neck + shoulders
            } else {
                float nx = (x - 15.5f) / 12.0f;
                float ny = (y - 10.5f) / 10.5f;
                if (ny < -0.35f) col = fam.skin[2];
                else if (ny > 0.35f) col = fam.skin[1];
                else if (nx <= -0.88f) col = fam.skin[1];
                else col = fam.skin[0];
            }
            setPixelMirrored(c, x, y, col);
        }
    }

    // Outline the silhouette.
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x <= 15; x++) {
            if (!inside[x][y]) continue;
            bool edge = !isFilled(c, x - 1, y) || !isFilled(c, x + 1, y) ||
                        !isFilled(c, x, y - 1) || !isFilled(c, x, y + 1);
            if (edge) setPixelMirrored(c, x, y, fam.outline);
        }
    }
}