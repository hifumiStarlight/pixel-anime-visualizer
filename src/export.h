#pragma once

#include "canvas.h"
#include <string>

struct ExportResult {
    bool ok = false;
    std::string path;  // valid when ok == true
    std::string error; // valid when ok == false
};

ExportResult exportAvatarPng(const Canvas& canvas);

// Backwards-compatible wrapper (deprecated — prefer ExportResult overload).
inline bool exportAvatarPng(const Canvas& canvas, std::string& savedPath,
                            std::string& error) {
    ExportResult r = exportAvatarPng(canvas);
    if (r.ok) {
        savedPath = r.path;
        return true;
    }
    error = r.error;
    return false;
}
