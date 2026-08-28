#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "avatar.h"
#include "export.h"
#include "ui.h"
#include <string>

namespace {

template <typename Enum>
bool cycleEnumButton(Rectangle bounds, const char* label, Enum& value, int count) {
    if (!GuiButton(bounds, label)) return false;
    int next = (static_cast<int>(value) + 1) % count;
    value = static_cast<Enum>(next);
    return true;
}

bool cycleButton(Rectangle bounds, const char* label, int& value, int count) {
    // Reuse the typed helper via a small adapter to avoid duplicating wrap logic.
    // int is not an enum, so we forward through the same modular arithmetic.
    if (!GuiButton(bounds, label)) return false;
    value = (value + 1) % count;
    return true;
}

} // namespace

bool drawControlPanel(Rectangle panel, AvatarConfig& config, uint32_t& rngState,
                      const Canvas& canvas, std::string& status, float& statusSeconds) {
    bool changed = false;
    const PaletteFamily& family = families[config.paletteFamily];
    const int x = static_cast<int>(panel.x);
    int y = static_cast<int>(panel.y);
    const int width = static_cast<int>(panel.width);
    const int rowHeight = 34;

    GuiPanel(panel, "Avatar controls");
    y += 42;
    GuiLabel({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 20 },
             "PALETTE FAMILY");
    y += 22;
    if (cycleButton({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 30 },
                     family.name, config.paletteFamily, kFamilyCount)) {
        config = normalizeAvatarConfig(config);
        changed = true;
    }
    y += rowHeight + 8;

    GuiLabel({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 20 },
             "HAIR");
    y += 22;
    if (cycleEnumButton({ static_cast<float>(x), static_cast<float>(y), width * 0.48f, 30 },
                     hairStyleName(config.hairStyle),
                     config.hairStyle, kHairStyleCount)) {
        changed = true;
    }
    if (cycleButton({ static_cast<float>(x + width * 0.52f), static_cast<float>(y), width * 0.48f, 30 },
                     "next color", config.hairColor, family.hairCount)) changed = true;
    y += rowHeight + 8;

    GuiLabel({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 20 },
             "EYES");
    y += 22;
    if (cycleEnumButton({ static_cast<float>(x), static_cast<float>(y), width * 0.48f, 30 },
                     eyeStyleName(config.eyeStyle),
                     config.eyeStyle, kEyeStyleCount)) changed = true;
    if (cycleButton({ static_cast<float>(x + width * 0.52f), static_cast<float>(y), width * 0.48f, 30 },
                     "next color", config.eyeColor, family.eyeCount)) changed = true;
    y += rowHeight + 12;

    if (GuiButton({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 36 },
                   "RANDOMIZE ALL")) {
        config = randomizeAll(rngState);
        status = "Randomized";
        statusSeconds = 1.5f;
        changed = true;
    }
    y += 48;
    if (GuiButton({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 36 },
                   "EXPORT PNG")) {
        ExportResult r = exportAvatarPng(canvas);
        if (r.ok) status = "Saved " + r.path;
        else status = r.error;
        statusSeconds = 2.5f;
    }
    y += 52;
    GuiLabel({ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 22 },
             TextFormat("seed: %u", config.seed));
    if (statusSeconds > 0.0f) {
        bool isError = !(status == "Randomized" || status.rfind("Saved ", 0) == 0);
        DrawText(status.c_str(), x, y + 30, 16, isError ? RED : GREEN);
    }
    return changed;
}
