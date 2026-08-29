#pragma once

#include "avatar.h"

// Persist AvatarConfig as a hand-editable key=value ini next to the exe.
// Colors are stored as int indices (B6), never raylib Colors or the Canvas
// buffer — AvatarConfig is the source of truth; pixels are derived.
void saveConfig(const char* path, const AvatarConfig& config);
bool loadConfig(const char* path, AvatarConfig& out); // false = missing/unusable