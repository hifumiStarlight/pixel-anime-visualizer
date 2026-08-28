#pragma once

#include "raylib.h"

struct Canvas {
    // Raw storage - sprite content must go through setPixelMirrored (generator.h)
    // to preserve horizontal symmetry. Direct set() bypasses the invariant.
    Color px[32 * 32];

    Color* data() { return px; }
    const Color* data() const { return px; }

    Color& get(int x, int y) { return px[y * 32 + x]; }
    const Color& get(int x, int y) const { return px[y * 32 + x]; }
    void set(int x, int y, Color c) { px[y * 32 + x] = c; }
    void clear(Color c) {
        for (int i = 0; i < 32 * 32; i++) px[i] = c;
    }

    // Non-owning Image view for raylib ExportImage / LoadTextureFromImage.
    // The Image does not copy pixels; caller must keep Canvas alive.
    Image toImage() const {
        return Image{ const_cast<Color*>(px), 32, 32, 1,
                      PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
    }
};