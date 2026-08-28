#pragma once

#include "raylib.h"

struct Canvas {
    Color px[32 * 32];

    Color& get(int x, int y) { return px[y * 32 + x]; }
    const Color& get(int x, int y) const { return px[y * 32 + x]; }
    void set(int x, int y, Color c) { px[y * 32 + x] = c; }
    void clear(Color c) {
        for (int i = 0; i < 32 * 32; i++) px[i] = c;
    }
};