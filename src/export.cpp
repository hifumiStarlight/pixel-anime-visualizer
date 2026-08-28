#include "export.h"

#include "raylib.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <string>

ExportResult exportAvatarPng(const Canvas& canvas) {
    namespace fs = std::filesystem;
    const fs::path directory = fs::path("exports");

    std::error_code ec;
    fs::create_directories(directory, ec);
    if (ec) {
        return ExportResult{ false, {}, "Cannot create exports/: " + ec.message() };
    }

    int nextIndex = 1;
    for (const fs::directory_entry& entry : fs::directory_iterator(directory, ec)) {
        if (ec) break;
        std::error_code ec2;
        if (!entry.is_regular_file(ec2) || ec2) continue;
        std::string name = entry.path().filename().string();
        // Match avatar_<digits>.png case-insensitively, without <regex>.
        if (name.size() < 11) continue; // at least avatar_1.png
        // prefix check (case-sensitive for "avatar_" — lower-case as written)
        if (name.rfind("avatar_", 0) != 0) continue;
        if (name.size() < 4) continue;
        std::string suffix = name.substr(name.size() - 4);
        for (char& c : suffix) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (suffix != ".png") continue;
        std::string middle = name.substr(7, name.size() - 7 - 4);
        if (middle.empty()) continue;
        bool allDigits = true;
        for (char c : middle) {
            if (!std::isdigit(static_cast<unsigned char>(c))) { allDigits = false; break; }
        }
        if (!allDigits) continue;
        try {
            long v = std::stol(middle);
            if (v >= 1 && v < 1000000) nextIndex = std::max(nextIndex, static_cast<int>(v) + 1);
        } catch (...) {
            continue;
        }
    }
    if (ec) {
        return ExportResult{ false, {}, "Cannot scan exports/: " + ec.message() };
    }

    const fs::path output = directory / (std::string("avatar_") +
        (nextIndex < 10 ? "00" : nextIndex < 100 ? "0" : "") +
        std::to_string(nextIndex) + ".png");

    Image image = canvas.toImage();
    if (!ExportImage(image, output.string().c_str())) {
        return ExportResult{ false, {}, "Could not write " + output.string() };
    }

    return ExportResult{ true, output.string(), {} };
}
