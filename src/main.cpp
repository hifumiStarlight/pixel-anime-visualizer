#include "raylib.h"
#include "canvas.h"
#include "palette.h"
#include "generator.h"
#include <cstdio>
#include <cstring>

static Texture2D canvasTexture(const Canvas& c) {
    Image img = { (void*)c.px, 32, 32, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
    Texture2D tex = LoadTextureFromImage(img);
    SetTextureFilter(tex, TEXTURE_FILTER_POINT);
    return tex;
}

static void dumpCanvas(const Canvas& c) {
    Color order[64];
    char sym[64];
    int n = 0;
    auto charFor = [&](Color col) -> char {
        for (int i = 0; i < n; i++) {
            if (order[i].r == col.r && order[i].g == col.g && order[i].b == col.b) return sym[i];
        }
        char s = 'A' + n;
        order[n] = col;
        sym[n] = s;
        n++;
        return s;
    };
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 32; x++) printf("%c", charFor(c.get(x, y)));
        printf("\n");
    }
}

static bool checkSymmetry(const Canvas& c) {
    bool ok = true;
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 16; x++) {
            Color a = c.get(x, y), b = c.get(31 - x, y);
            if (a.r != b.r || a.g != b.g || a.b != b.b || a.a != b.a) {
                printf("ASYMMETRY at (%d,%d)\n", x, y);
                ok = false;
            }
        }
    }
    printf("symmetry: %s\n", ok ? "OK" : "BROKEN");
    return ok;
}

int main(int argc, char** argv) {
    const char* shot = nullptr;
    bool dump = false;
    bool faceGrid = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--shot") == 0 && i + 1 < argc) shot = argv[i + 1];
        if (strcmp(argv[i], "--dump") == 0) dump = true;
        if (strcmp(argv[i], "--face-grid") == 0) faceGrid = true;
    }

    Canvas faces[kFamilyCount];
    for (int f = 0; f < kFamilyCount; f++) {
        drawFace(faces[f], families[f]);
        if (dump) {
            printf("== %s ==\n", families[f].name);
            checkSymmetry(faces[f]);
            dumpCanvas(faces[f]);
        }
    }
    const PaletteFamily& family = families[0];
    constexpr int styleCount = static_cast<int>(EyeStyle::COUNT);
    Canvas eyeFaces[styleCount];
    for (int i = 0; i < styleCount; i++) {
        drawFace(eyeFaces[i], family);
        drawEyes(eyeFaces[i], static_cast<EyeStyle>(i), family.eyeColors[0], family);
    }
    if (dump) {
        const char* labels[styleCount] = { "round", "sharp", "sleepy" };
        for (int i = 0; i < styleCount; i++) {
            printf("== %s eyes ==\n", labels[i]);
            checkSymmetry(eyeFaces[i]);
        }
        return 0;
    }

    if (faceGrid) {
        InitWindow(824, 340, "Pixel Anime Avatar Generator - face");
        SetTargetFPS(60);
        Texture2D faceTextures[kFamilyCount];
        for (int f = 0; f < kFamilyCount; f++) faceTextures[f] = canvasTexture(faces[f]);

        const int scale = 8;
        Rectangle src = { 0, 0, 32, 32 };
        Rectangle dst = { 0, 40, 32 * scale, 32 * scale };
        int frame = 0;
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground({ 28, 28, 34, 255 });
            for (int f = 0; f < kFamilyCount; f++) {
                dst.x = 12 + f * (32 * scale + 16);
                DrawTexturePro(faceTextures[f], src, dst, { 0, 0 }, 0.0f, WHITE);
                DrawText(families[f].name, dst.x, 8, 20, WHITE);
            }
            EndDrawing();
            if (shot && frame == 30) {
                TakeScreenshot(shot);
                break;
            }
            frame++;
        }
        for (int f = 0; f < kFamilyCount; f++) UnloadTexture(faceTextures[f]);
        CloseWindow();
        return 0;
    }

    InitWindow(824, 340, "Pixel Anime Avatar Generator - eyes");
    SetTargetFPS(60);

    Texture2D tex[styleCount];
    for (int i = 0; i < styleCount; i++) tex[i] = canvasTexture(eyeFaces[i]);

    const int scale = 8;
    Rectangle src = { 0, 0, 32, 32 };
    Rectangle dst = { 0, 40, 32 * scale, 32 * scale };

    int frame = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({ 28, 28, 34, 255 });
        for (int i = 0; i < styleCount; i++) {
            dst.x = 12 + i * (32 * scale + 16);
            DrawTexturePro(tex[i], src, dst, { 0, 0 }, 0.0f, WHITE);
            const char* label = i == 0 ? "round" : i == 1 ? "sharp" : "sleepy";
            DrawText(label, dst.x, 8, 20, WHITE);
        }
        EndDrawing();
        if (shot && frame == 30) {
            TakeScreenshot(shot);
            break;
        }
        frame++;
    }

    for (int i = 0; i < styleCount; i++) UnloadTexture(tex[i]);
    CloseWindow();
    return 0;
}
