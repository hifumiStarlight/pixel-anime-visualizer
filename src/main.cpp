#include "raylib.h"
#include "avatar.h"
#include "export.h"
#include "ui.h"
#include <cstdio>
#include <cstring>
#include <string>

static Texture2D uploadCanvas(const Canvas& canvas) {
    Image image = { const_cast<Color*>(canvas.px), 32, 32, 1,
                    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
    Texture2D texture = LoadTextureFromImage(image);
    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    return texture;
}

int main(int argc, char** argv) {
    const char* screenshot = nullptr;
    bool exportTest = false;
    for (int i = 1; i + 1 < argc; i++) {
        if (strcmp(argv[i], "--shot") == 0) screenshot = argv[i + 1];
    }
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--export-test") == 0) exportTest = true;
    }

    AvatarConfig config = {
        0, HairStyle::Spiky, 1, EyeStyle::Round, 0, 0xC0FFEEu
    };
    uint32_t rngState = 0x12345678u;
    Canvas canvas = renderAvatar(config);

    if (exportTest) {
        std::string savedPath;
        std::string error;
        if (!exportAvatarPng(canvas, savedPath, error)) {
            std::fprintf(stderr, "%s\n", error.c_str());
            return 1;
        }
        std::printf("exported: %s\n", savedPath.c_str());
        return 0;
    }

    InitWindow(1000, 620, "Pixel Anime Avatar Generator");
    SetTargetFPS(60);
    Texture2D texture = uploadCanvas(canvas);
    int frame = 0;
    std::string status;
    float statusSeconds = 0.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({ 24, 24, 32, 255 });
        DrawText("PIXEL ANIME AVATAR", 28, 18, 24, RAYWHITE);
        DrawTexturePro(texture, { 0, 0, 32, 32 }, { 28, 58, 512, 512 },
                       { 0, 0 }, 0.0f, WHITE);
        DrawRectangleLines(28, 58, 512, 512, { 90, 90, 105, 255 });

        bool changed = drawControlPanel({ 580, 18, 390, 560 }, config, rngState,
                                         canvas, status, statusSeconds);
        EndDrawing();

        if (changed) {
            config = normalizeAvatarConfig(config);
            canvas = renderAvatar(config);
            UnloadTexture(texture);
            texture = uploadCanvas(canvas);
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
