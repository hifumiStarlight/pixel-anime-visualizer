#pragma once

#include "canvas.h"
#include <string>

bool exportAvatarPng(const Canvas& canvas, std::string& savedPath,
                    std::string& error);
