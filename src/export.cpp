#include "export.h"

#include "raylib.h"
#include <algorithm>
#include <filesystem>
#include <regex>

bool exportAvatarPng(const Canvas& canvas, std::string& savedPath,
                    std::string& error) {
    namespace fs = std::filesystem;
    const fs::path directory = fs::path("exports");

    std::error_code ec;
    fs::create_directories(directory, ec);
    if (ec) {
        error = "Cannot create exports/: " + ec.message();
        return false;
    }

    int nextIndex = 1;
    const std::regex filenamePattern(R"(^avatar_([0-9]+)\.png$)",
                                      std::regex::icase);
    for (const fs::directory_entry& entry : fs::directory_iterator(directory, ec)) {
        if (ec) break;
        std::smatch match;
        const std::string name = entry.path().filename().string();
        if (entry.is_regular_file(ec) && std::regex_match(name, match, filenamePattern)) {
            nextIndex = std::max(nextIndex, std::stoi(match[1].str()) + 1);
        }
    }
    if (ec) {
        error = "Cannot scan exports/: " + ec.message();
        return false;
    }

    const fs::path output = directory / (std::string("avatar_") +
        (nextIndex < 10 ? "00" : nextIndex < 100 ? "0" : "") +
        std::to_string(nextIndex) + ".png");
    Image image = { const_cast<Color*>(canvas.px), 32, 32, 1,
                    PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
    if (!ExportImage(image, output.string().c_str())) {
        error = "Could not write " + output.string();
        return false;
    }

    savedPath = output.string();
    return true;
}
