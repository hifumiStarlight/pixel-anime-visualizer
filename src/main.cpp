#include "raylib.h"
#include "avatar.h"
#include "export.h"
#include "ui.h"
#include <cstdio>
#include <cstring>
#include <string>

static Texture2D uploadCanvas(const Canvas& canvas) {
    Image image = canvas.toImage();
    Texture2D texture = LoadTextureFromImage(image);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    return texture;
}

static void refreshTexture(Texture2D& texture, const Canvas& canvas) {
    // 32x32 is tiny — UpdateTexture avoids the Unload/Load churn.
    Image image = canvas.toImage();
    UpdateTexture(texture, image.data);
}

int main(int argc, char** argv) {
    const char* screenshot = nullptr;
    bool exportTest = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--shot") == 0) {
            if (i + 1 < argc && strncmp(argv[i + 1], "--", 2) != 0) screenshot = argv[i + 1];
            else std::fprintf(stderr, "warning: --shot requires a file path\n");
        }
        if (strcmp(argv[i], "--export-test") == 0) exportTest = true;
    }

    AvatarConfig config = {
        0, HairStyle::Spiky, 1, EyeStyle::Round, 0, 0xC0FFEEu
    };
    uint32_t rngState = 0x12345678u;
    Canvas canvas = renderAvatar(config);

    if (exportTest) {
        ExportResult r = exportAvatarPng(canvas);
        if (!r.ok) {
            std::fprintf(stderr, "%s\n", r.error.c_str());
            return 1;
        }
        std::printf("exported: %s\n", r.path.c_str());
        return 0;
    }

    InitWindow(1000, 620, "Pixel Anime Avatar Generator");
    SetTargetFPS(60);
    Texture2D texture = uploadCanvas(canvas);
    int frame = 0;
    std::string status;
    float statusSeconds = 0.0f;

    while (!WindowShouldClose()) {
        // Keyboard shortcuts (mouse-free workflow): R randomize, E export.
        bool shortcutChanged = false;
        if (IsKeyPressed(KEY_R)) {
            config = randomizeAll(rngState);
            status = "Randomized";
            statusSeconds = 1.5f;
            shortcutChanged = true;
        }
        if (IsKeyPressed(KEY_E)) {
            ExportResult r = exportAvatarPng(canvas);
            if (r.ok) status = "Saved " + r.path;
            else status = r.error;
            statusSeconds = 2.5f;
        }

        BeginDrawing();
        ClearBackground({ 24, 24, 32, 255 });
        DrawText("PIXEL ANIME AVATAR", 28, 18, 24, RAYWHITE);
        DrawTexturePro(texture, { 0, 0, 32, 32 }, { 28, 58, 512, 512 },
                       { 0, 0 }, 0.0f, WHITE);
        DrawRectangleLines(28, 58, 512, 512, { 90, 90, 105, 255 });

        bool panelChanged = drawControlPanel({ 580, 18, 390, 560 }, config, rngState,
                                             canvas, status, statusSeconds);
        EndDrawing();

        bool changed = shortcutChanged || panelChanged;
        if (changed) {
            config = normalizeAvatarConfig(config);
            canvas = renderAvatar(config);
            refreshTexture(texture, canvas);
        }
        if (statusSeconds > 0.0f) statusSeconds -= GetFrameTime();
        if (screenshot && frame++ == 30) {
            TakeScreenshot(screenshot);
            break;
        }
    }

    UnloadTexture(texture);
    CloseWindow();
    return 0;
}
