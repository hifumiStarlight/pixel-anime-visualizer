cmake_minimum_required(VERSION 3.20)
project(pixel_anime_visualizer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG 4.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raygui)

add_executable(pixel_anime_visualizer
    src/main.cpp
    src/canvas.h
    src/palette.h
    src/palette.cpp
    src/generator.h
    src/generator.cpp
    src/eyes.cpp
    src/hair.cpp
    src/mouth.cpp
    src/avatar.h
    src/avatar.cpp
    src/config_io.h
    src/config_io.cpp
    src/rng.h
    src/ui.cpp
    src/ui.h
    src/export.cpp
    src/export.h
)
target_link_libraries(pixel_anime_visualizer PRIVATE raylib)
# raygui is header-only and intentionally noisy under /W4; treat as SYSTEM
# so its warnings don't break our build.
target_include_directories(pixel_anime_visualizer SYSTEM PRIVATE ${raygui_SOURCE_DIR}/src)

# Warning hygiene — MSVC /W4, GCC/Clang -Wall -Wextra
if (MSVC)
    target_compile_options(pixel_anime_visualizer PRIVATE /W4 /permissive- /wd4996)
    # /wd4996 silences raygui's fopen/sprintf deprecation noise when it is
    # (transitively) included as non-SYSTEM via raylib interop; SYSTEM above
    # handles most of it but MSVC still warns on CRT deprecation.
else()
    target_compile_options(pixel_anime_visualizer PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Note: WIN32_EXECUTABLE is intentionally not set. Forcing /SUBSYSTEM:WINDOWS
# requires a WinMain entry point and breaks the --export-test CLI path.
# The console window is expected and documented in README.
