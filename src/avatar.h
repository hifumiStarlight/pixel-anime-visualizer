#pragma once

#include "generator.h"
#include <cstdint>

struct AvatarConfig {
    int paletteFamily;
    HairStyle hairStyle;
    int hairColor;
    EyeStyle eyeStyle;
    int eyeColor;
    MouthStyle mouthStyle;
    uint32_t seed;
};

bool isValidAvatarConfig(const AvatarConfig& config);
AvatarConfig normalizeAvatarConfig(AvatarConfig config);
Canvas renderAvatar(const AvatarConfig& config);
AvatarConfig randomizeAll(uint32_t& rngState);
