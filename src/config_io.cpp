#include "config_io.h"

#include <cerrno>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void saveConfig(const char* path, const AvatarConfig& config) {
    FILE* f = std::fopen(path, "w");
    if (!f) return;
    std::fprintf(f, "version=1\n");
    std::fprintf(f, "paletteFamily=%d\n", config.paletteFamily);
    std::fprintf(f, "hairStyle=%d\n", static_cast<int>(config.hairStyle));
    std::fprintf(f, "hairColor=%d\n", config.hairColor);
    std::fprintf(f, "eyeStyle=%d\n", static_cast<int>(config.eyeStyle));
    std::fprintf(f, "eyeColor=%d\n", config.eyeColor);
    std::fprintf(f, "mouthStyle=%d\n", static_cast<int>(config.mouthStyle));
    std::fprintf(f, "seed=%u\n", config.seed);
    std::fclose(f);
}

bool loadConfig(const char* path, AvatarConfig& out) {
    FILE* f = std::fopen(path, "r");
    if (!f) return false;

    AvatarConfig parsed = out; // missing keys keep the caller's default
    bool anyKnown = false;
    char line[256];
    while (std::fgets(line, sizeof(line), f)) {
        char* eq = std::strchr(line, '=');
        if (!eq) continue;
        *eq = '\0';
        char* key = line;
        char* value = eq + 1;

        while (*key == ' ' || *key == '\t') key++;
        while (*value == ' ' || *value == '\t') value++;
        size_t keyLength = std::strlen(key);
        while (keyLength > 0) {
            char c = key[keyLength - 1];
            if (c != ' ' && c != '\t') break;
            key[--keyLength] = '\0';
        }
        for (size_t i = std::strlen(value); i > 0; i--) {
            char c = value[i - 1];
            if (c != '\n' && c != '\r' && c != ' ' && c != '\t') break;
            value[i - 1] = '\0';
        }
        if (*key == '\0' || *value == '\0') continue;

        if (std::strcmp(key, "version") == 0) continue; // informational, not used yet

        bool knownKey = std::strcmp(key, "paletteFamily") == 0 ||
                        std::strcmp(key, "hairStyle") == 0 ||
                        std::strcmp(key, "hairColor") == 0 ||
                        std::strcmp(key, "eyeStyle") == 0 ||
                        std::strcmp(key, "eyeColor") == 0 ||
                        std::strcmp(key, "mouthStyle") == 0 ||
                        std::strcmp(key, "seed") == 0;
        if (!knownKey) continue; // unknown keys, including nonnumeric values, are ignored

        char* end = nullptr;
        errno = 0;
        unsigned long long uv = std::strtoull(value, &end, 10);
        if (end == value || *end != '\0' || errno == ERANGE) {
            std::fclose(f);
            return false;
        }
        const bool seedKey = std::strcmp(key, "seed") == 0;
        if ((!seedKey && uv > static_cast<unsigned long long>(INT_MAX)) ||
            (seedKey && uv > UINT32_MAX)) {
            std::fclose(f);
            return false;
        }

        if (std::strcmp(key, "paletteFamily") == 0) {
            parsed.paletteFamily = static_cast<int>(uv);
        } else if (std::strcmp(key, "hairStyle") == 0) {
            parsed.hairStyle = static_cast<HairStyle>(uv);
        } else if (std::strcmp(key, "hairColor") == 0) {
            parsed.hairColor = static_cast<int>(uv);
        } else if (std::strcmp(key, "eyeStyle") == 0) {
            parsed.eyeStyle = static_cast<EyeStyle>(uv);
        } else if (std::strcmp(key, "eyeColor") == 0) {
            parsed.eyeColor = static_cast<int>(uv);
        } else if (std::strcmp(key, "mouthStyle") == 0) {
            parsed.mouthStyle = static_cast<MouthStyle>(uv);
        } else if (std::strcmp(key, "seed") == 0) {
            parsed.seed = static_cast<uint32_t>(uv);
        }
        anyKnown = true;
    }
    std::fclose(f);
    if (!anyKnown) return false;

    // The file is never trusted: normalize and require a clean round-trip.
    // Out-of-range indices, a stale hairColor for the wrong family, or any
    // other garbage makes isValidAvatarConfig fail -> caller falls back.
    if (!isValidAvatarConfig(parsed)) return false;
    out = normalizeAvatarConfig(parsed);
    return true;
}
