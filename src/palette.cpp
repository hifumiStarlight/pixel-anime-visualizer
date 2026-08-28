#include "palette.h"

static Color hex(int h) {
    return { (unsigned char)((h >> 16) & 0xFF),
             (unsigned char)((h >> 8) & 0xFF),
             (unsigned char)(h & 0xFF),
             255 };
}

static_assert(kFamilyCount == 3, "families size must match palette.h");

const PaletteFamily families[kFamilyCount] = {
    // warm / classic shounen: peachy skin, black-brown-to-red hair
    {
        "warm",
        { hex(0xF7C9A3), hex(0xD99A6C), hex(0xFFE3C8) },
        5,
        { hex(0x2A1E1C), hex(0x4A2C24), hex(0x7A4A2A), hex(0xA63D2F), hex(0x5A2A3A) },
        4,
        { hex(0x4A3424), hex(0x8A5A3A), hex(0x2E4057), hex(0x3E5C2E) },
        hex(0x3A2A22),
    },
    // cool / pastel: pale skin, blue-lavender-mint hair
    {
        "cool",
        { hex(0xFFE4DE), hex(0xF0B8AC), hex(0xFFF4EF) },
        5,
        { hex(0xA7C7E7), hex(0xC3B1E1), hex(0xB5EAD7), hex(0xD9E0FF), hex(0xE7C6E0) },
        4,
        { hex(0x7BA7D1), hex(0x9B8CC4), hex(0x6FB5A0), hex(0xE0A9C6) },
        hex(0x4A3A4A),
    },
    // deep / high-contrast: rich brown skin, warm gold and violet accents
    {
        "deep",
        { hex(0x8B5A3C), hex(0x5E3826), hex(0xA9755A) },
        5,
        { hex(0x2E2418), hex(0x5A3A22), hex(0xC79A3E), hex(0x7A4A8A), hex(0xA8542E) },
        4,
        { hex(0xC79A3E), hex(0x9B6B9B), hex(0x6B4A2E), hex(0x8A3A4A) },
        hex(0x241A12),
    },
};
