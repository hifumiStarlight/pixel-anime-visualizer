#include "avatar.h"
#include "rng.h"

AvatarConfig normalizeAvatarConfig(AvatarConfig config) {
    if (config.paletteFamily < 0 || config.paletteFamily >= kFamilyCount) config.paletteFamily = 0;
    const PaletteFamily& family = families[config.paletteFamily];
    if (static_cast<int>(config.hairStyle) < 0 || static_cast<int>(config.hairStyle) >= kHairStyleCount)
        config.hairStyle = HairStyle::Spiky;
    if (static_cast<int>(config.eyeStyle) < 0 || static_cast<int>(config.eyeStyle) >= kEyeStyleCount)
        config.eyeStyle = EyeStyle::Round;
    if (static_cast<int>(config.mouthStyle) < 0 || static_cast<int>(config.mouthStyle) >= kMouthStyleCount)
        config.mouthStyle = MouthStyle::Neutral;
    if (config.hairColor < 0 || config.hairColor >= family.hairCount) config.hairColor = 0;
    if (config.eyeColor < 0 || config.eyeColor >= family.eyeCount) config.eyeColor = 0;
    return config;
}

bool isValidAvatarConfig(const AvatarConfig& config) {
    AvatarConfig n = normalizeAvatarConfig(config);
    return n.paletteFamily == config.paletteFamily &&
           n.hairStyle == config.hairStyle &&
           n.eyeStyle == config.eyeStyle &&
           n.mouthStyle == config.mouthStyle &&
           n.hairColor == config.hairColor &&
           n.eyeColor == config.eyeColor;
}

Canvas renderAvatar(const AvatarConfig& input) {
    AvatarConfig config = normalizeAvatarConfig(input);
    const PaletteFamily& family = families[config.paletteFamily];
    Canvas canvas;
    drawFace(canvas, family);
    drawHair(canvas, config.hairStyle, family.hairColors[config.hairColor], config.seed, family);
    drawEyes(canvas, config.eyeStyle, family.eyeColors[config.eyeColor], family);
    // Mouth drawn last so it sits on skin and can never be overdrawn by hair.
    drawMouth(canvas, config.mouthStyle, family);
    return canvas;
}

AvatarConfig randomizeAll(uint32_t& rngState) {
    AvatarConfig config{};
    config.paletteFamily = avatar_rng::bounded(rngState, kFamilyCount);
    const PaletteFamily& family = families[config.paletteFamily];
    config.hairColor = avatar_rng::bounded(rngState, family.hairCount);
    config.eyeColor = avatar_rng::bounded(rngState, family.eyeCount);
    config.hairStyle = static_cast<HairStyle>(avatar_rng::bounded(rngState, kHairStyleCount));
    config.eyeStyle = static_cast<EyeStyle>(avatar_rng::bounded(rngState, kEyeStyleCount));
    config.mouthStyle = static_cast<MouthStyle>(avatar_rng::bounded(rngState, kMouthStyleCount));
    config.seed = avatar_rng::next(rngState);
    return config;
}
