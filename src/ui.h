#pragma once

#include "avatar.h"
#include <string>

bool drawControlPanel(Rectangle panel, AvatarConfig& config, uint32_t& rngState,
                      const Canvas& canvas, std::string& status,
                      float& statusSeconds);
